import unreal
import os
import importlib.util
import subprocess

PLUGIN_PATH = "VUEDK/Core/DialogueSystem"

DEFAULT_ENGINE_FULL_PATH = unreal.Paths.project_config_dir() + "DefaultEngine.ini"
TARGET_SECTION = "[/Script/PythonScriptPlugin.PythonScriptPluginSettings]"
SCRIPTS_PATH = unreal.Paths.project_plugins_dir() + PLUGIN_PATH + "/Content/Python"
PACKAGES_FULL_PATH = SCRIPTS_PATH + "/packages.txt"
PATHS_TO_INCLUDE_FULL_PATH = SCRIPTS_PATH + "/paths_to_include.txt"


def is_package_installed(package_name):
    return importlib.util.find_spec(package_name) is not None


def install_packages():
    python_exe = unreal.get_interpreter_executable_path()

    # List of required packages
    packages = []
    with open(PACKAGES_FULL_PATH, "r") as file:
        packages = [line.strip() for line in file.readlines()]

    missing_packages = [pkg for pkg in packages if not is_package_installed(pkg)]

    if not missing_packages:
        unreal.log("All packages are already installed.")
        return

    unreal.log(f"Installing missing packages: {', '.join(missing_packages)}")

    subprocess.run(
        [python_exe, "-m", "pip", "install"] + missing_packages,
        check=True,
        creationflags=subprocess.CREATE_NO_WINDOW
    )

    unreal.log("All packages installed successfully.")


def add_path_to_config(path):
    new_path_added = False
    ini_path = DEFAULT_ENGINE_FULL_PATH
    new_entry = "+AdditionalPaths=(Path=\"" + path.strip() + "\")"

    if not os.path.exists(ini_path):
        print(f"DefaultEngine.ini not found at {ini_path}.")
        return None

    with open(ini_path, "r", encoding="utf-8") as file:
        lines = file.readlines()

    inside_target_section = False
    entry_exists = False
    new_lines = []

    for line in lines:
        if line.strip() == TARGET_SECTION:
            inside_target_section = True
        elif line.startswith("[") and inside_target_section:
            inside_target_section = False

        if inside_target_section and new_entry in line:
            entry_exists = True

        new_lines.append(line)

    if not entry_exists:
        new_path_added = True
        for i, line in enumerate(new_lines):
            if line.strip() == TARGET_SECTION:
                j = i + 1
                while j < len(new_lines) and not new_lines[j].startswith("["):
                    j += 1
                new_lines.insert(j, "\n" + new_entry + "\n")
                break

        with open(ini_path, "w", encoding="utf-8") as file:
            file.writelines(new_lines)

        print(f"Added {new_entry} to {ini_path}.")

    return new_path_added


def add_paths_from_file():
    any_new_path_added = False
    with open(PATHS_TO_INCLUDE_FULL_PATH, "r") as file:
        lines = file.readlines()
        for line in lines:
            any_new_path_added |= add_path_to_config(SCRIPTS_PATH + "/" + line.strip())

    return any_new_path_added


def show_restart_dialog():
    unreal.EditorDialog.show_message(
        "Restart Required",
        "The Python environment has been updated. Restart the Unreal Editor to apply changes.",
        unreal.AppMsgType.OK_CANCEL
    )


# === EXECUTION FLOW ===

if __name__ == "__main__":
    should_restart = False
    should_restart |= add_path_to_config(SCRIPTS_PATH)
    should_restart |= add_paths_from_file()

    if should_restart:
        show_restart_dialog()

    install_packages()
