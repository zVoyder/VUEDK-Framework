import os.path
import unreal
import drive_downloader as dd

ASSET_TOOLS = unreal.AssetToolsHelpers.get_asset_tools()


def create_or_get_data_asset(asset_name, destination_path, asset_class):
    full_path = destination_path + f"/{asset_name}"

    # Check if the interlocutor already exists
    if unreal.EditorAssetLibrary.does_asset_exist(full_path):
        unreal.log(f"'{full_path}' already exists. Updating...")
        return unreal.load_asset(full_path)

    return create_data_asset(asset_name, destination_path, asset_class)


def create_data_asset(asset_name, destination_path, asset_class):
    full_path = destination_path + f"/{asset_name}"

    new_asset = ASSET_TOOLS.create_asset(
        asset_name,
        destination_path,
        asset_class,
        unreal.DataAssetFactory()
    )

    if new_asset is None:
        unreal.log_error(f"Data Asset could not be created: '{asset_name}'.")
        return None

    unreal.log(f"Data Asset '{asset_name}' successfully created in '{destination_path}'.")
    unreal.EditorAssetLibrary.save_asset(full_path)
    return new_asset


def create_unreal_asset_from_drive(asset_name, asset_path, share_link, factory, file_extension, task=unreal.AssetImportTask()):
    temp_path = f"temp.{file_extension}"
    dd.download_file_from_google_drive(share_link, temp_path)

    import_task = task
    import_task.filename = temp_path
    import_task.destination_name = asset_name
    import_task.destination_path = asset_path
    import_task.automated = True
    import_task.replace_existing = True
    import_task.save = True

    import_task.factory = factory
    ASSET_TOOLS.import_asset_tasks([import_task])

    os.remove(temp_path)
    full_path = asset_path + f"/{asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(full_path) is False:
        unreal.log_error(f"Asset could not be created: '{asset_name}', import error.")
        return None

    unreal.log(f"Asset '{asset_name}' successfully created in '{asset_path}'.")
    return unreal.load_asset(full_path)
