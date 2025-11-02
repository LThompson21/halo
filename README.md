# HALO

**HALO** is a low-level C++ framework for building modern Windows applications.  
It provides a minimal, high-performance foundation for initialization, error handling, and core system integration — all implemented as a **shared library (DLL)**.

> ⚠️ **Platform Support:**  
> HALO currently builds and runs **only on Windows**.  
> It is **always built as a shared library** (`.dll`). Static builds are not supported.

---

## 🧩 Features

- Simple initialization and teardown API (`halo::Init`, `halo::Terminate`)
- Centralized error management (`halo::SetError`, `halo::GetError`)
- Thread-safe initialization guard (`halo::IsMainThread`)
- Versioning utilities (`halo::Version`)
- Lightweight, header-first design with a single implementation unit
- Explicit DLL export/import model (`__declspec(dllexport)` / `__declspec(dllimport)`)

---

## 🏗️ Building

Requires:
- **Windows 10 or later**
- **CMake ≥ 3.21**
- **Clang or MSVC** (Clang recommended)
- **Ninja** (optional, for faster builds)

### Example

```powershell
git clone https://github.com/LThompson21/halo.git
cd halo

cmake -S . -B out --preset Debug
cmake --build out --target halo
