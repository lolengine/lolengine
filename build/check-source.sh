#!/bin/sh

set -e

fix=false
quiet=false
while [ "$#" -gt 0 ]; do
    case "$1" in
      -q)
        quiet=true
        ;;
      -w)
        fix=true
        ;;
      -c)
        commit=true
        fix=true
        quiet=true
        ;;
      *)
        echo "E: invalid argument $1"
        exit 1
        ;;
    esac
    shift
done

error() { if [ "$quiet" != true ]; then echo "E: $1"; fi }
info() { if [ "$quiet" != true ]; then echo "I: $1"; fi }

# Ensure the system tools do not attempt to perform multibyte conversions
export LANG=C

# Find out where the top directory is and go there
top_srcdir="$(cd "$(dirname $0)"; cd ..; pwd)"
cd "$top_srcdir"

# Check for working tools
if [ "$(echo foo | grep -c foo)" != 1 ]; then
    error "grep -c does not appear to work, cancelling"
    exit 0
fi

SED=sed
if gsed --version >/dev/null 2>&1; then
    SED=gsed
fi
if [ "$(echo 'x\x' | $SED 's/.*[^x\t]//')" != x ]; then
    error "sed does not appear to work, cancelling"
    exit 0
fi

if d2u -h >/dev/null 2>&1; then
    d2u=d2u
elif dos2unix -h >/dev/null 2>&1; then
    d2u=dos2unix
else
    error "d2u or dos2unix not found, cancelling"
    exit 0
fi

# Find out whether we need to care about CRLF
case "$(uname 2>/dev/null)" in
  MINGW*)
    check_crlf=false
    ;;
  *)
    check_crlf=true
    ;;
esac

# Find out what kind of Vcs directory this is
if [ -f "$top_srcdir/.git/index" ]; then
    info "detected Git repository"
    repo=git
elif [ -f "$top_srcdir/.svn/format" ]; then
    info "detected SVN repository"
    repo=svn
else
    info "not in a Vcs directory, nothing to do"
    exit 0
fi

total_crlfs=0
total_spaces=0
total_tabs=0

OIFS="$IFS"
IFS='
'
if [ "$repo" = git ]; then
    FILES="`git ls-files`"
else
    FILES="`svn ls -R`"
fi

total_files=0
for file in $FILES; do
    if [ -f "$file" ]; then
        total_files="$(($total_files + 1))"
    fi
done

total_errors=0
for file in $FILES; do
    case "$file" in
      # These files aren't ours, don't fix
      src/bullet/*|\
      src/lua/*|\
      external/*|\
      */generated/*|\
      web/plugins/*)
          :
          ;;
      # Don't harass these people
      people/peeweek/*|\
      people/touky/*|\
      people/benlitz/*|\
      people/sam/lua-*)
          :
          ;;
      # These files we know how to handle
      *.c|*.cpp|*.m|*.mm|*.h|*.hh|*.lolfx|*.lua|*.l|*.y|*.sh|*.py)
          clean=true

          # Check for CR LF
          if [ "$check_crlf" = true ]; then
              ncrlfs="$(od -tx1 "$file" | cut -b8- | tr ' ' '\n' | grep -c 0d || true)"
              total_crlfs="$(($total_crlfs + $ncrlfs))"
              if [ "$ncrlfs" -gt 0 ]; then
                  clean=false
                  if [ "$fix" = true ]; then
                      $d2u -q "$file"
                      info "$file has $ncrlfs CR characters"
                  else
                      error "$file has $ncrlfs CR characters"
                  fi
              fi
          fi

          # Check for trailing spaces
          nspaces="$($SED 's/.*[^ \t]//' "$file" | tr -cd '\t ' | wc -c)"
          total_spaces="$(($total_spaces + $nspaces))"
          if [ "$nspaces" -gt 0 ]; then
              clean=false
              if [ "$fix" = true ]; then
                  $SED -i 's/[[:space:]][[:space:]]*$//g' "$file"
                  info "$file has $nspaces trailing spaces"
              else
                  error "$file has $nspaces trailing spaces"
              fi
          fi

          # Check for tabs
          ntabs="$(tr -cd '\t' < "$file" | wc -c)"
          total_tabs="$(($total_tabs + $ntabs))"
          if [ "$ntabs" -gt 0 ]; then
              clean=false
              if [ "$fix" = true ]; then
                  $SED -i 's/\t/    /g' "$file"
                  info "$file has $ntabs tabs"
              else
                  error "$file has $ntabs tabs"
              fi
          fi

          if [ "$clean" != true ]; then
              total_errors="$(($total_errors + 1))"
          fi
          ;;
    esac
done
IFS="$OIFS"

if [ "$total_errors" -gt 0 ]; then
    if [ "$commit" = "true" ]; then
        # EITHER: commit all modified files
        svn commit --username lolbot --non-interactive -F - << EOF
fixed $total_errors files out of $total_files:
 - removed $total_crlfs CR characters
 - removed $total_spaces trailing whitespaces
 - replaced $total_tabs tabs with spaces
EOF
    elif [ "$fix" = "true" ]; then
        # OR: report in stdout
        info "fixed $total_errors files out of $total_files:"
        if [ "$total_crlfs" -gt 0 ]; then
            info " - fixed $total_crlfs CR characters"
        fi
        if [ "$total_spaces" -gt 0 ]; then
            info " - fixed $total_spaces trailing spaces"
        fi
        if [ "$total_tabs" -gt 0 ]; then
            info " - fixed $total_tabs tabs"
        fi
        info "re-run with -c to commit fixes"
    else
        # OR: warn about how to fix errors
        info "re-run with -w to fix errors"
    fi
else
    info "all $total_files source files appear to be OK, congratulations"
fi

