#!/usr/bin/env python3
"""Minimal OpenAI-compatible chat client (works for DeepSeek and OpenAI).

API key is read from the environment, or from config/secrets.env (gitignored) of the
form  KEY=value  per line. The key never needs to be passed on the command line.
"""
import json
import os
from pathlib import Path

import requests

STUDY_ROOT = Path(__file__).resolve().parent.parent
MODELS = json.loads((STUDY_ROOT / "config/models.json").read_text())


def _load_secrets():
    f = STUDY_ROOT / "config/secrets.env"
    if not f.is_file():
        return
    for line in f.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        k, v = line.split("=", 1)
        os.environ.setdefault(k.strip(), v.strip().strip('"').strip("'"))


def get_key(model_cfg: dict) -> str:
    _load_secrets()
    env = model_cfg["api_key_env"]
    key = os.environ.get(env)
    if not key:
        raise RuntimeError(
            f"missing API key: set {env} in config/secrets.env (e.g. {env}=sk-...)")
    return key


class LLMClient:
    def __init__(self, model_name: str):
        if model_name not in MODELS:
            raise KeyError(f"unknown model {model_name}; have {list(MODELS)}")
        self.name = model_name
        self.cfg = MODELS[model_name]

    def chat(self, messages, timeout=120, seed=None):
        cfg = self.cfg
        payload = {"model": cfg["model"], "messages": messages,
                   "temperature": cfg["temperature"], "max_tokens": cfg["max_tokens"]}
        if seed is not None:
            payload["seed"] = seed   # reproducible-but-varying sampling across runs
        r = requests.post(
            f"{cfg['base_url']}/chat/completions",
            headers={"Authorization": f"Bearer {get_key(cfg)}",
                     "Content-Type": "application/json"},
            json=payload,
            timeout=timeout,
        )
        r.raise_for_status()
        data = r.json()
        usage = data.get("usage", {})
        return {
            "text": data["choices"][0]["message"]["content"],
            "tokens_in": usage.get("prompt_tokens"),
            "tokens_out": usage.get("completion_tokens"),
            "model_version": data.get("model", cfg["model"]),
            "temperature": cfg["temperature"],
            "raw": data,   # full API response — the paid artifact, persisted verbatim
        }
