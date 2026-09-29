#!/bin/bash
git filter-branch -f --env-filter '
if [ "$GIT_AUTHOR_NAME" = "Ryouichiwatanabe" ]; then
    export GIT_AUTHOR_NAME="SoTanaka0409"
    export GIT_AUTHOR_EMAIL="tky2502039@stu.o-hara.ac.jp"
fi
if [ "$GIT_COMMITTER_NAME" = "Ryouichiwatanabe" ]; then
    export GIT_COMMITTER_NAME="SoTanaka0409"
    export GIT_COMMITTER_EMAIL="tky2502039@stu.o-hara.ac.jp"
fi
' HEAD
