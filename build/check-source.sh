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

# Find out where the top directory is and go there
top_srcdir="$(cd "$(dirname $0)"; cd ..; pwd)"
cd "$top_srcdir"

# Check for working tools
#if [ "$(echo foo | grep -c foo)" != 1 ]; then
#    error "grep -c does not appear to work, cancelling"
#    exit 0
#fi
if d2u -h >/dev/null 2>&1; then
    d2u=d2u
elif dos2unix -h >/dev/null 2>&1; then
    d2u=dos2unix
else
    error "d2u or dos2unix not found, cancelling"
    exit 0
fi

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

total_errors=0
total_files=0
total_crs=0
total_spaces=0
total_tabs=0

OIFS="$IFS"
IFS=$'\n'
if [ "$repo" = git ]; then
    FILES="`git ls-files`"
else
    FILES="`svn ls -R`"
fi

for file in $FILES; do
    case "$file" in
      src/bullet/*|contrib/*|*/generated/*)
          : # These files aren't ours, don't fix
          ;;
      people/peeweek/*|people/touky/*)
          : # Don't harass these people
          ;;
      *.c|*.cpp|*.h|*.l|*.y)
          clean=true

          # Check for CR LF
          ncrs="$(od -tx1 "$file" | cut -b8- | tr ' ' '\n' | grep -c 0d || true)"
          total_crs="$(($total_crs + $ncrs))"
          if [ "$ncrs" -gt 0 ]; then
              clean=false
              if [ "$fix" = true ]; then
                  $d2u -q "$file"
                  info "$file has $ncrs CR characters"
              else
                  error "$file has $ncrs CR characters"
              fi
          fi

          # Check for trailing spaces
          nspaces="$(sed 's/.*[^ \t]//' "$file" | tr -cd '\t ' | wc -c)"
          total_spaces="$(($total_spaces + $nspaces))"
          if [ "$nspaces" -gt 0 ]; then
              clean=false
              if [ "$fix" = true ]; then
                  sed -i 's/[[:space:]][[:space:]]*$//g' "$file"
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
                  sed -i 's/\t/    /g' "$file"
                  info "$file has $ntabs tabs"
              else
                  error "$file has $ntabs tabs"
              fi
          fi

          total_files="$(($total_files + 1))"
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
 - fixed $total_crs CR characters
 - fixed $total_spaces trailing spaces
 - fixed $total_tabs tabs
EOF
    elif [ "$fix" = "true" ]; then
        # OR: report in stdout
        info "fixed $total_errors files out of $total_files:"
        if [ "$total_crs" -gt 0 ]; then
            info " - fixed $total_crs CR characters"
        fi
        if [ "$total_spaces" -gt 0 ]; then
            info " - fixed $total_spaces trailing spaces"
        fi
        if [ "$total_tabs" -gt 0 ]; then
            info " - fixed $total_tabs tabs"
        fi
    else
        # OR: warn about how to fix errors
        info "re-run with -w to fix errors"
    fi
fi

