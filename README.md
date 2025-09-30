# R-Type

R-Type est une implémentation moderne du célèbre shoot’em up des années 80, construite en **C++**, avec un **moteur maison**, un **client** et un **serveur** réseau.
Le projet suit une architecture modulaire pour favoriser la maintenance, les tests et l’évolution du code.

---

## 🚀 Fonctionnalités principales

* **Moteur de jeu** (gestion des entités, composants, systèmes).
* **Architecture client/serveur** pour le multijoueur.
* **Gestion des ressources** (sprites, audio, effets).
* **Tests unitaires** intégrés.
* **Documentation Doxygen** et conventions de commits.

---

## 📂 Organisation du projet

```
.
├── assets/               # Ressources (audio, sprites, backgrounds, effets…)
├── engine/               # Cœur du moteur de jeu
│   ├── include/          # Headers du moteur
│   ├── src/              # Sources du moteur
│   └── tests/            # Tests unitaires du moteur
├── mains/                # Points d’entrée client et serveur
├── rtype_client/         # Code spécifique du client
├── rtype_server/         # Code spécifique du serveur
├── utility_classes/      # Classes utilitaires (réseau, factory…)
├── CMakeLists.txt        # Build principal
├── conanfile.txt         # Dépendances Conan
├── compiling.sh          # Script de compilation
├── COMMIT_CONVENTION.md  # Conventions de commit
├── development_workflow.md # Workflow de développement
├── Doxyfile              # Configuration Doxygen
└── protocol.md           # Spécification du protocole réseau
```

---

## 🛠️ Prérequis

* **C++20** ou plus
* **CMake ≥ 3.15**
* **Conan** (gestion des dépendances)
* **Make**
* Un compilateur compatible (g++, clang++)

---

## ⚙️ Compilation

### Ou manuellement

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

---

## ▶️ Exécution

Le projet suit une architecture **client/serveur**.

### Lancer le serveur

```bash
./mains/server/main
```

### Lancer le client

```bash
./mains/client/main
```

*(Tu peux aussi créer des configurations dans ton IDE pour lancer les deux binaires.)*

---

## 🧪 Tests

Chaque module contient un dossier `tests` avec des tests unitaires.
Pour compiler et exécuter :

```bash
cmake --build . --target r-type_tests   # exemple pour le moteur
./engine/tests/registry_tests           # exécution
```

---

## 📚 Documentation

* **Documentation Doxygen** :

  ```bash
  doxygen Doxyfile
  ```

  La documentation sera générée dans `docs/html`.

* **Conventions de commit** : voir `COMMIT_CONVENTION.md`

* **Workflow de développement** : voir `development_workflow.md`

* **Protocole réseau** : voir `protocol.md`

---

## 🤝 Contribution

1. Forker le dépôt
2. Créer une branche feature :

   ```bash
   git checkout -b feature/ma-feature
   ```
3. Committer selon `COMMIT_CONVENTION.md`
4. Push et ouvrir une Pull Request
