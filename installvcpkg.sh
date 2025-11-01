#!/usr/bin/env bash

# ==========================================================
# Installation et configuration automatique de vcpkg
# Compatible Linux / macOS / Windows (Git Bash / WSL)
# ==========================================================

REPO_URL="https://github.com/microsoft/vcpkg.git"

# --- Détection du système ---
OS="$(uname -s)"
case "$OS" in
    Linux*)     PLATFORM="linux" ;;
    Darwin*)    PLATFORM="mac" ;;
    MINGW*|MSYS*|CYGWIN*) PLATFORM="windows" ;;
    *)          PLATFORM="unknown" ;;
esac

echo "🧩 Plateforme détectée : $PLATFORM"

# --- Dossier d’installation par défaut ---
if [ "$PLATFORM" = "windows" ]; then
    VCPKG_DIR="$USERPROFILE\\vcpkg"
    SHELL_RC="$USERPROFILE\\.bashrc"
    TRIPLET_DEFAULT="x64-windows"
else
    VCPKG_DIR="$HOME/vcpkg"
    SHELL_RC="$HOME/.bashrc"
    TRIPLET_DEFAULT="x64-linux"
fi

# --- Vérification de vcpkg ---
echo "🔍 Vérification de vcpkg..."

if command -v vcpkg &> /dev/null; then
    echo "✅ vcpkg déjà installé : $(which vcpkg)"
else
    echo "⚙️ vcpkg non trouvé, installation en cours..."

    # Clonage du dépôt
    if [ ! -d "$VCPKG_DIR" ]; then
        echo "📦 Clonage du dépôt dans $VCPKG_DIR..."
        git clone "$REPO_URL" "$VCPKG_DIR" || { echo "❌ Erreur de clonage."; exit 1; }
    fi

    # Construction
    cd "$VCPKG_DIR" || exit 1
    if [ "$PLATFORM" = "windows" ]; then
        echo "🔧 Bootstrap pour Windows..."
        ./bootstrap-vcpkg.bat
    else
        echo "🔧 Bootstrap pour Linux/macOS..."
        ./bootstrap-vcpkg.sh
    fi

    echo "✅ vcpkg installé avec succès."
fi

# --- Configuration des variables d’environnement ---
echo "🌍 Configuration de l’environnement..."

if [ "$PLATFORM" = "windows" ]; then
    # Pour PowerShell (Windows)
    POWERSHELL_PROFILE="$(powershell.exe -NoLogo -NoProfile -Command '$PROFILE' | tr -d '\r')"

    echo "📄 Ajout des variables à ton profil PowerShell : $POWERSHELL_PROFILE"

    powershell.exe -NoProfile -Command "
        \$ProfilePath = '$POWERSHELL_PROFILE';
        if (!(Test-Path \$ProfilePath)) { New-Item -ItemType File -Path \$ProfilePath -Force | Out-Null }
        Add-Content -Path \$ProfilePath -Value ''
        Add-Content -Path \$ProfilePath -Value '# --- vcpkg configuration ---';
        Add-Content -Path \$ProfilePath -Value 'Set-Item -Path Env:VCPKG_ROOT \"$VCPKG_DIR\"';
        Add-Content -Path \$ProfilePath -Value 'Set-Item -Path Env:VCPKG_DEFAULT_TRIPLET \"$TRIPLET_DEFAULT\"';
        Add-Content -Path \$ProfilePath -Value 'setx PATH (\$Env:PATH + \";$VCPKG_DIR\")';
        Add-Content -Path \$ProfilePath -Value '# --- end vcpkg configuration ---';
    "
else
    # Pour Linux / macOS
    export VCPKG_ROOT="$VCPKG_DIR"
    export PATH="$PATH:$VCPKG_DIR"
    export VCPKG_DEFAULT_TRIPLET="$TRIPLET_DEFAULT"

    if ! grep -q "VCPKG_ROOT" "$SHELL_RC"; then
        echo "" >> "$SHELL_RC"
        echo "# --- vcpkg configuration ---" >> "$SHELL_RC"
        echo "export VCPKG_ROOT=\"$VCPKG_DIR\"" >> "$SHELL_RC"
        echo "export PATH=\"\$PATH:\$VCPKG_ROOT\"" >> "$SHELL_RC"
        echo "export VCPKG_DEFAULT_TRIPLET=\"$TRIPLET_DEFAULT\"" >> "$SHELL_RC"
        echo "# --- end vcpkg configuration ---" >> "$SHELL_RC"
    fi
fi

echo ""
echo "🎉 Configuration terminée !"
echo "➡️ Ouvre un nouveau terminal et teste avec :"
echo "   vcpkg version"
echo ""

