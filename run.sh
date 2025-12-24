#!/usr/bin/env bash
set -euo pipefail

# Project paths
PROJECT_DIR="/Users/k2/Desktop/appStore"
SRC="${PROJECT_DIR}/backend.cpp"
BIN="${PROJECT_DIR}/backend"

# Ensure brew exists
if ! command -v brew >/dev/null 2>&1; then
  echo "[ERROR] Homebrew not found. Install it first:"
  echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
  exit 1
fi

BREW_PREFIX="$(brew --prefix)"
MYSQL_CLIENT_PREFIX="${BREW_PREFIX}/opt/mysql-client"
INCLUDE_DIR="${MYSQL_CLIENT_PREFIX}/include"
LIB_DIR="${MYSQL_CLIENT_PREFIX}/lib"
MYSQLADMIN="${MYSQL_CLIENT_PREFIX}/bin/mysqladmin"

# Verify mysql-client is installed
if [[ ! -d "${MYSQL_CLIENT_PREFIX}" ]]; then
  echo "[ERROR] mysql-client not found at ${MYSQL_CLIENT_PREFIX}"
  echo "Install it with: brew install mysql-client"
  exit 1
fi

# Verify headers and libs
if [[ ! -d "${INCLUDE_DIR}" ]] || [[ ! -d "${LIB_DIR}" ]]; then
  echo "[ERROR] Missing include/lib directories under ${MYSQL_CLIENT_PREFIX}"
  echo "Tried:"
  echo "  ${INCLUDE_DIR}"
  echo "  ${LIB_DIR}"
  exit 1
fi

# Ensure source exists
if [[ ! -f "${SRC}" ]]; then
  echo "[ERROR] Source file not found: ${SRC}"
  exit 1
fi

echo "[BUILD] Compiling ${SRC} -> ${BIN}"
g++ -std=c++17 "${SRC}" \
  -o "${BIN}" \
  -I"${INCLUDE_DIR}" \
  -L"${LIB_DIR}" \
  -lmysqlclient \
  -Wl,-rpath,"${LIB_DIR}"

echo "[CHECK] MySQL server ping (optional)"
if [[ -x "${MYSQLADMIN}" ]]; then
  if ! "${MYSQLADMIN}" ping -h127.0.0.1 -uroot -pnewlatiospower >/dev/null 2>&1; then
    echo "[WARN] mysqladmin ping failed. Ensure MySQL is running and credentials are correct."
  else
    echo "[OK] mysqladmin ping succeeded."
  fi
else
  echo "[INFO] mysqladmin not available; skipping ping."
fi

echo "[RUN] ${BIN}"
exec "${BIN}"


