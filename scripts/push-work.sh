#!/usr/bin/env bash
set -euo pipefail

message="${*:-Agent update}"
repo_root="$(git rev-parse --show-toplevel 2>/dev/null || true)"

if [[ -z "$repo_root" ]]; then
    echo "Error: run this from inside the PluginLab git repo."
    exit 1
fi

cd "$repo_root"

echo "Repo: $repo_root"
echo "Branch: $(git branch --show-current)"
echo
echo "Current status:"
git status --short

if [[ -z "$(git status --porcelain)" ]]; then
    echo "No changes to commit."
    exit 0
fi

echo
echo "Staging tracked and untracked source/docs/script changes..."
git add -A

if git diff --cached --quiet; then
    echo "No committable changes after applying ignore rules."
    exit 0
fi

echo
echo "Files staged for commit:"
git diff --cached --name-status

echo
echo "Committing: $message"
git commit -m "$message"

branch="$(git branch --show-current)"
if [[ -z "$branch" ]]; then
    echo "Error: cannot push from a detached HEAD."
    exit 1
fi

echo
echo "Pushing to origin/$branch..."
git push -u origin "$branch"

echo
echo "Done. VPS source changes are on GitHub."
