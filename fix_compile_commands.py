import json
import os
import re

def fix_compile_commands():
    root_dir = "/home/low/Desktop/cyberpunk2077-dll-compile"
    win_root = "D:/a/cyberpunk2077-dll-compile/cyberpunk2077-dll-compile"
    win_root_bs = win_root.replace("/", "\\")

    db_path = os.path.join(root_dir, "build/compile_commands.json")
    if not os.path.exists(db_path):
        print(f"Error: {db_path} not found")
        return

    with open(db_path, "r") as f:
        data = json.load(f)

    # Flags to completely remove (MSVC-only)
    msvc_remove_exact = {
        "/nologo", "/TP", "/GR", "/EHsc", "/MD", "/FS",
        "/O2", "/Ob2", "/DNDEBUG",
    }

    fixed_data = []
    for entry in data:
        # Fix directory
        d = entry["directory"]
        d = d.replace(win_root, root_dir).replace(win_root_bs, root_dir)
        d = d.replace("\\", "/")
        entry["directory"] = d

        # Fix file
        fl = entry["file"]
        fl = fl.replace(win_root, root_dir).replace(win_root_bs, root_dir)
        fl = fl.replace("\\", "/")
        entry["file"] = fl

        # Fix command
        cmd = entry["command"]
        cmd = cmd.replace(win_root, root_dir)
        cmd = cmd.replace(win_root_bs, root_dir)
        cmd = cmd.replace("\\", "/")

        # Convert -std:c++20 -> -std=c++20
        cmd = cmd.replace("-std:c++20", "-std=c++20")

        # Split into parts and filter
        parts = cmd.split()
        filtered = []
        for p in parts:
            # Replace compiler
            if "cl.exe" in p and p == parts[0]:
                filtered.append("clang++")
                continue

            # Remove exact MSVC-only flags
            if p in msvc_remove_exact:
                continue

            # Remove /Fo..., /Fd... output flags (with or without path)
            if re.match(r"^/F[od]", p, re.IGNORECASE):
                continue

            # Convert /DXXX -> -DXXX (MSVC define syntax)
            if p.startswith("/D"):
                filtered.append("-D" + p[2:])
                continue

            # Convert /IXXX -> -IXXX (MSVC include syntax)
            if p.startswith("/I"):
                filtered.append("-I" + p[2:])
                continue

            filtered.append(p)

        entry["command"] = " ".join(filtered)
        fixed_data.append(entry)

    out_path = os.path.join(root_dir, "compile_commands.json")
    with open(out_path, "w") as f:
        json.dump(fixed_data, f, indent=2)

    print(f"Fixed compile_commands.json -> {out_path}")
    print(f"  Total entries: {len(fixed_data)}")
    if fixed_data:
        cmd_parts = fixed_data[0]['command'].split()
        print(f"\nFirst entry command preview:")
        print("  " + " ".join(cmd_parts[:10]) + " ...")

if __name__ == "__main__":
    fix_compile_commands()
