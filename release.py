import os
import shutil
import sys
from pathlib import Path

def main():
    version = input("Enter release version (e.g., 1.0.0): ").strip()
    if not version:
        print("Error: Version cannot be empty.")
        sys.exit(1)

    project_name = "dmath"
    release_name = f"{project_name}-v{version}-universal"
    release_dir = Path(release_name)

    # 1. Create release folder structure
    if release_dir.exists():
        shutil.rmtree(release_dir)
    
    release_dir.mkdir(parents=True)

    # 2. Copy 'include' folder
    print("[*] Copying include directory...")
    root_include = Path("include")
    if root_include.exists():
        shutil.copytree(root_include, release_dir / "include")
        print("    Copied: include/")
    else:
        print("[!] Warning: 'include' directory not found in root.")

    # 3. Copy README and LICENSE
    print("[*] Copying documentation...")
    for doc in ["README.md", "LICENSE"]:
        doc_path = Path(doc)
        if doc_path.exists():
            shutil.copy(doc_path, release_dir)
            print(f"    Copied: {doc}")
        else:
            print(f"[!] Warning: '{doc}' not found in root.")

    # 4. Zip the release folder and clean up raw directory
    print(f"[*] Packaging into {release_name}.zip...")
    shutil.make_archive(release_name, 'zip', root_dir='.', base_dir=release_name)
    shutil.rmtree(release_dir)

    print(f"\n[+] Success! Created {release_name}.zip")

if __name__ == "__main__":
    main()
