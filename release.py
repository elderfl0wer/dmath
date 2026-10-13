import os
import shutil
import subprocess
import sys
from pathlib import Path

def main():
    version = input("Enter release version (e.g., 1.0.0): ").strip()
    if not version:
        print("Error: Version cannot be empty.")
        sys.exit(1)

    project_name = "dmath"
    release_name = f"{project_name}-v{version}-windows"
    release_dir = Path(release_name)
    build_dir = Path("build")

    # 1. Configure (if needed) and build the project via CMake
    print("[*] Checking build configuration & compiling...")
    if not build_dir.exists():
        subprocess.run(["cmake", "-B", "build", "-DCMAKE_BUILD_TYPE=Release"], check=True)
    
    subprocess.run(["cmake", "--build", "build", "--config", "Release"], check=True)

    # 2. Create release folder structure
    if release_dir.exists():
        shutil.rmtree(release_dir)
    
    lib_dest = release_dir / "lib"
    lib_dest.mkdir(parents=True)

    # 3. Collect static library binaries (.lib / .a)
    print("[*] Collecting library binaries...")
    possible_output_dirs = [
        build_dir,
        build_dir / "Release",
        build_dir / "Debug"
    ]
    
    lib_found = False
    for src_dir in possible_output_dirs:
        if src_dir.exists():
            for file in src_dir.glob("*.lib"):
                shutil.copy(file, lib_dest)
                print(f"    Copied: {file.name}")
                lib_found = True
            for file in src_dir.glob("*.a"):
                shutil.copy(file, lib_dest)
                print(f"    Copied: {file.name}")
                lib_found = True

    if not lib_found:
        print("[!] Warning: No static library files (.lib/.a) found in standard build output paths.")

    # 4. Copy 'include' folder
    print("[*] Copying include directory...")
    root_include = Path("include")
    if root_include.exists():
        shutil.copytree(root_include, release_dir / "include")
    else:
        print("[!] Warning: 'include' directory not found in root.")

    # 5. Copy README and LICENSE
    print("[*] Copying documentation...")
    for doc in ["README.md", "LICENSE"]:
        if Path(doc).exists():
            shutil.copy(doc, release_dir)
            print(f"    Copied: {doc}")

    # 6. Zip the release folder and clean up raw directory
    print(f"[*] Packaging into {release_name}.zip...")
    shutil.make_archive(release_name, 'zip', root_dir='.', base_dir=release_name)
    shutil.rmtree(release_dir)

    print(f"\n[+] Success! Created {release_name}.zip")

if __name__ == "__main__":
    main()
