import shutil
import sys
import zipfile
from pathlib import Path


def main():
    version = input("Enter release version (e.g., 1.0.0): ").strip()
    if not version:
        print("Error: Version cannot be empty.")
        sys.exit(1)

    project_name = "dmath"
    release_name = f"{project_name}-v{version}-universal"
    release_dir = Path(release_name)

    # 1. Create temporary release folder
    if release_dir.exists():
        shutil.rmtree(release_dir)

    release_dir.mkdir(parents=True)

    # 2. Copy include directory
    print("[*] Copying include directory...")
    include_dir = Path("include")
    if include_dir.exists():
        shutil.copytree(include_dir, release_dir / "include")
        print("    Copied: include/")
    else:
        print("[!] Warning: 'include' directory not found.")

    # 3. Copy documentation
    print("[*] Copying documentation...")
    for doc in ("README.md", "LICENSE"):
        path = Path(doc)
        if path.exists():
            shutil.copy2(path, release_dir)
            print(f"    Copied: {doc}")
        else:
            print(f"[!] Warning: '{doc}' not found.")

    # 4. Create ZIP containing only the contents of release_dir
    zip_path = Path(f"{release_name}.zip")
    print(f"[*] Packaging into {zip_path.name}...")

    if zip_path.exists():
        zip_path.unlink()

    with zipfile.ZipFile(zip_path, "w", compression=zipfile.ZIP_DEFLATED) as archive:
        for path in release_dir.rglob("*"):
            archive.write(path, path.relative_to(release_dir))

    # 5. Remove temporary release directory
    shutil.rmtree(release_dir)

    print(f"\n[+] Success! Created {zip_path.name}")


if __name__ == "__main__":
    main()
