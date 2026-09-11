# --- Git branch function ---
parse_git_branch() {
    git branch 2>/dev/null | sed -n '/\* /s///p'
}

# --- Color definitions ---
RESET='\[\033[00m\]'
GREEN='\[\033[01;32m\]'
BLUE='\[\033[01;34m\]'
YELLOW='\[\033[01;33m\]'

# --- Prompt: user@host:path (branch) $ ---
PS1="${GREEN}\u@\h${RESET}:${BLUE}\w${RESET}${YELLOW}\$(parse_git_branch)${RESET}\$ "
