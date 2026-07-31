# Model Provenance (replication record)

Commercial model names and endpoints drift over time; this file pins exactly what was
called, where, when, and with which parameters. Source of truth for the runs is
`config/models.json` (consumed by `generate/llm_client.py`) plus the raw per-cell
responses shipped under `data/raw*/` — **the shipped raw outputs, not provider-side
determinism, are the reproducibility anchor** (see "Seed support" below).

All calls go through the OpenAI-compatible `/chat/completions` API of each endpoint.

## Campaign models

| Alias in data | Paper name | Provider | Endpoint (`base_url`) | API model ID | Access dates (2026) | Temp | Max tokens | Price in/out ($/Mtok) |
|---|---|---|---|---|---|---|---|---|
| `deepseek-chat` | DeepSeek-V4-flash | DeepSeek | `https://api.deepseek.com` | `deepseek-chat` | Jun 23–26 | 0.2 | 4096 | 0.14 / 0.28 |
| `qwen3-coder` | Qwen3-Coder-30B | Alibaba via OpenRouter | `https://openrouter.ai/api/v1` | `qwen/qwen3-coder-30b-a3b-instruct` | Jun 24–26 | 0.2 | 4096 | 0.07 / 0.27 |
| `gemini-2.5-flash` | Gemini-2.5-Flash | Google via OpenRouter | `https://openrouter.ai/api/v1` | `google/gemini-2.5-flash` | Jun 24–26 | 0.2 | 4096 | 0.15 / 0.60 |
| `gpt-5.4-or` | GPT-5.4 (frontier) | OpenAI via OpenRouter | `https://openrouter.ai/api/v1` | `openai/gpt-5.4` | Jun 24 (Tier A), Jun 26 (cpp-peglib) | 0.2 | 4096 | 2.50 / 15.00 |

Sensitivity arms (same DeepSeek model, different temperature): `deepseek-chat-t00`
(temp 0.0) and `deepseek-chat-t08` (temp 0.8), run Jun 23 on 4 fmt defects.

## Access dates by campaign (from FINDINGS.md entries)

- Tier A, DeepSeek 3 runs fixed+buggy: **2026-06-23** (C1/C2)
- Tier A, Qwen + Gemini: **2026-06-24** (C7)
- Tier A, GPT-5.4: **2026-06-24** (C15 Exp1)
- Internal campaigns v1/v2 + SPIRV (3 models): **2026-06-24** (C13/C15/C16)
- Arrow public probe + `table_builder` probe (3 models): **2026-06-24 / 2026-06-25** (C14/C19)
- BugsCpp `cpp-peglib` (4 models): **2026-06-26** (C20)

## Seed support

`generate/llm_client.py` sends `seed = run_index` in the request payload for every
call (OpenAI-compatible `seed` field). Adherence is **not guaranteed by providers**:
we empirically observed DeepSeek returning different completions across identical
greedy (temp 0.0) requests (FINDINGS C3), and OpenRouter forwards `seed` to
underlying providers on a best-effort basis. Reproducibility therefore rests on the
shipped raw model outputs, not on re-sampling.

## Deprecation / drift notes

- DeepSeek announced `deepseek-chat` deprecates **2026-07-24**; the same model is
  reachable as `deepseek-v4-flash` afterwards (entry present in `models.json`).
- OpenRouter slugs (`qwen/…`, `google/…`, `openai/…`) are the IDs actually called;
  vendor-native endpoints may use different IDs (e.g. DashScope for Qwen,
  `generativelanguage.googleapis.com` for Gemini).
