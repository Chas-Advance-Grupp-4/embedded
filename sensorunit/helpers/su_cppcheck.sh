cppcheck \
  --enable=warning,style,performance,portability \
  --inconclusive \
  --std=c++11 \
  --force \
  --quiet \
  --error-exitcode=1 \
  -I include \
  -I lib \
  src lib include