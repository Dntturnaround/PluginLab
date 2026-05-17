#!/usr/bin/env bash
set -euo pipefail

if [ $# -lt 2 ]; then
  echo "Usage: $0 <agent-name> <task>"
  echo "Example: $0 bishop-qa 'Review ProGainUtility build readiness'"
  exit 1
fi

AGENT_NAME="$1"
shift
TASK="$*"
LAB_ROOT="/home/edward/PluginLab"
AGENT_FILE="$LAB_ROOT/agents/hermes-agents/$AGENT_NAME.md"

if [ ! -f "$AGENT_FILE" ]; then
  echo "Unknown agent: $AGENT_NAME"
  echo "Available agents:"
  ls "$LAB_ROOT/agents/hermes-agents" | sed 's/.md$//' | sed 's/^/  - /'
  exit 1
fi

PROMPT="$(cat "$AGENT_FILE")

## Current task
$TASK
"

cd "$LAB_ROOT"
hermes chat --quiet --toolsets terminal,file -q "$PROMPT"
