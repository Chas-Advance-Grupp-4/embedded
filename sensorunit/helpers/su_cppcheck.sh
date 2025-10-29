cppcheck \
  --enable=warning,style,performance,portability \
  --inconclusive \
  --std=c++11 \
  --force \
  --quiet \
  -I include \
  -I lib \
  src lib include