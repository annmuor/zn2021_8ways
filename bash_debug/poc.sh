#!/bin/sh
debug_poc() {
  echo "$USER($UID:$GROUPS) ran $BASH_COMMAND" >> /tmp/commands.log
}
trap debug_poc DEBUG

