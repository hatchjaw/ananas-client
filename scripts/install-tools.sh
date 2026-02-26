#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
ORANGE='\033[38;5;208m'
NC='\033[0m' # No Color

declare -A L

echo -e "${CYAN}╔══════════════════════════════╗${NC}"
echo -e "${CYAN}║${NC}  ${YELLOW}★  Installing tools...   ★${NC}  ${CYAN}║${NC}"
echo -e "${CYAN}╚══════════════════════════════╝${NC}"

#=============================== PlatformIO ====================================

# Uppercase
L[P,0]="█████ "; L[P,1]="██  ██"; L[P,2]="█████ "; L[P,3]="██    "; L[P,4]="██    "; L[P,5]=" ░░   "
L[I,0]="████ "; L[I,1]=" ██  "; L[I,2]=" ██  "; L[I,3]=" ██  "; L[I,4]="████ "; L[I,5]=" ░░░░"
L[O,0]=" ████ "; L[O,1]="██  ██"; L[O,2]="██  ██"; L[O,3]="██  ██"; L[O,4]=" ████ "; L[O,5]="  ░░░░"

# Lowercase — shorter glyphs, sitting on the baseline (rows 2-4, padded top)
L[l,0]=" ██  "; L[l,1]=" ██  "; L[l,2]=" ██  "; L[l,3]=" ██  "; L[l,4]=" ▀██ "; L[l,5]="   ░░░"
L[a,0]="        "; L[a,1]=" ▄████  "; L[a,2]="██  ██  "; L[a,3]="██  ██  "; L[a,4]=" ██████ "; L[a,5]="  ░░░░"
L[t,0]=" ██   "; L[t,1]="████  "; L[t,2]=" ██   "; L[t,3]=" ██   "; L[t,4]=" ▝██  "; L[t,5]="    ░░"
L[f,0]=" ▄██ "; L[f,1]="██   "; L[f,2]="████ "; L[f,3]="██   "; L[f,4]="██   "; L[f,5]="  ░░ "
L[o,0]="       "; L[o,1]=" ████  "; L[o,2]="██  ██ "; L[o,3]="██  ██ "; L[o,4]=" ████  "; L[o,5]="   ░░░░"
L[r,0]="     "; L[r,1]="█▄██ "; L[r,2]="██   "; L[r,3]="██   "; L[r,4]="██   "; L[r,5]="  ░░  "
L[m,0]="        "; L[m,1]="▄▄█▄█▄▄ "; L[m,2]="██ █ ██ "; L[m,3]="██   ██ "; L[m,4]="██   ██ "; L[m,5]=" ░░   ░░"

LETTERS=(P l a t f o r m I O)

echo ""

for row in 0 1 2 3 4 5; do
  line="  "
  for letter in "${LETTERS[@]}"; do
    line+="${L[$letter,$row]} "
  done
  echo -e "${ORANGE}${line}${NC}"
done

echo ""

cd ../tools || exit
curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py
rm -rf get-platformio.py

echo -e "${GREEN}╔══════════════════════════════╗${NC}"
echo -e "${GREEN}║${NC}          ${MAGENTA}★  "Done" ★         ${NC}  ${GREEN}║${NC}"
echo -e "${GREEN}╚══════════════════════════════╝${NC}"

#================================= TyTools =====================================

# Uppercase
L[T,0]="██████"; L[T,1]="  ██  "; L[T,2]="  ██  "; L[T,3]="  ██  "; L[T,4]="  ██  "; L[T,5]="   ░░ "; L[T,6]="      "; L[T,7]="      "

# Lowercase — shorter glyphs, sitting on the baseline (rows 2-4, padded top)
L[y,0]="       "; L[y,1]="▝█   ██"; L[y,2]=" ██  ██"; L[y,3]=" ██  ██"; L[y,4]="  ▝████"; L[y,5]="    ██ "; L[y,6]=" █████▘"; L[y,7]="  ░░░░░"
L[o,0]="       "; L[o,1]=" ████  "; L[o,2]="██  ██ "; L[o,3]="██  ██ "; L[o,4]=" ████  "; L[o,5]="  ░░░░ "; L[o,6]="       "; L[o,7]="       "
L[l,0]="██  "; L[l,1]="██  "; L[l,2]="██  "; L[l,3]="██  "; L[l,4]="▝██ "; L[l,5]=" ░░  "; L[l,6]="    "; L[l,7]="    "
L[s,0]="       "; L[s,1]="▄▄█████"; L[s,2]="██▄▄▄▄ "; L[s,3]=" ▀▀▀▀██"; L[s,4]="██████▀"; L[s,5]="░░░░░░ "; L[s,6]="       "; L[s,7]="       "

LETTERS=(T y T o o l s)

echo ""

for row in 0 1 2 3 4 5 6 7; do
  line="  "
  for letter in "${LETTERS[@]}"; do
    line+="${L[$letter,$row]} "
  done
  echo -e "${BLUE}${line}${NC}"
done

echo ""

sleep 2

cd ./rygel || exit
rm -rf ./bin
rm ./felix
./bootstrap.sh
./felix -pFast tycmd
echo -e "  ${CYAN}Copying tycmd to ~/.local/bin${NC}"
cp bin/Fast/tycmd ~/.local/bin

echo -e "${GREEN}╔══════════════════════════════╗${NC}"
echo -e "${GREEN}║${NC}          ${MAGENTA}★  "Done" ★         ${NC}  ${GREEN}║${NC}"
echo -e "${GREEN}╚══════════════════════════════╝${NC}"

#================================ Faust ========================================

L[F,0]="██████"; L[F,1]="██    "; L[F,2]="█████ "; L[F,3]="██    "; L[F,4]="██    "; L[F,5]=" ░░    "
L[a,0]="        "; L[a,1]=" ▄████  "; L[a,2]="██  ██  "; L[a,3]="██  ██  "; L[a,4]=" ██████ "; L[a,5]=" ░░░░░"
L[u,0]="       "; L[u,1]="██  ██ "; L[u,2]="██  ██ "; L[u,3]="██  ██ "; L[u,4]=" █████▄"; L[u,5]="   ░░░░░"
L[s,0]="       "; L[s,1]="▄▄█████"; L[s,2]="██▄▄▄▄ "; L[s,3]=" ▀▀▀▀██"; L[s,4]="██████▀"; L[s,5]=" ░░░░░░"
L[t,0]=" ██   "; L[t,1]="████  "; L[t,2]=" ██   "; L[t,3]=" ██   "; L[t,4]=" ▝██  "; L[t,5]="   ░░ "

LETTERS=(F a u s t)

echo ""

for row in 0 1 2 3 4 5; do
  line="  "
  for letter in "${LETTERS[@]}"; do
    line+="${L[$letter,$row]} "
  done
  echo -e "${RED}${line}${NC}"
done

echo ""

sleep 2

cd ../faust || exit
make distclean
make
sudo make install

echo ""

echo -e "${CYAN}╔═══════════════════════════════╗${NC}"
echo -e "${CYAN}║${NC}  ${YELLOW}★  "All tools installed ✨"  ★${NC}  ${CYAN}║${NC}"
echo -e "${CYAN}╚═══════════════════════════════╝${NC}"

cd ../../scripts || exit
