# PluginLab Hermes Agents

These are real Hermes-ready role prompts plus a launcher script.

## Available agents

- `king-coordinator`
- `product-strategist`
- `juce-architect`
- `dsp-engineer`
- `ui-ux-designer`
- `build-doctor`
- `cross-platform-release-engineer`
- `bishop-qa`

## Run an agent

```bash
/home/edward/PluginLab/scripts/run-plugin-agent.sh bishop-qa "Review ProGainUtility build readiness"
```

Another example:

```bash
/home/edward/PluginLab/scripts/run-plugin-agent.sh build-doctor "Verify ProGainUtility builds and report exact artifacts"
```

## Notes

- These run as one-shot Hermes agents using `hermes chat -q`.
- They have terminal/file tools enabled.
- They work from `/home/edward/PluginLab`.
- For long-running independent agents later, we can add tmux launchers or Hermes profiles.
