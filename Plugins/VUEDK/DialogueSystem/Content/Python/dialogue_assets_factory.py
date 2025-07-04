import unreal
from unreal_python_utility import assets_factory as af
from unreal_python_utility import google_sheet_downloader as gsd
from unreal_python_utility import google_id_helper as gih

INTERLOCUTORS_SHEET_RANGE = "Interlocutors!A2:Z"
DIALOGUES_RANGE = "!A2:Z"
INTERLOCUTORS_FOLDER_PATH = "/Interlocutors"
NODES_FOLDER_PATH = "/Nodes"
CHOICES_FOLDER_PATH = "/Choices"


def create_dialogue(spreadsheet_link, dialogue_sheet_name, destination_path, container_class, interlocutor_class, single_node_class, choice_node_class, choice_class):
    dialogue_path = destination_path + f"/{dialogue_sheet_name}"
    interlocutors_path = destination_path + INTERLOCUTORS_FOLDER_PATH
    nodes_path = dialogue_path + NODES_FOLDER_PATH
    choices_path = nodes_path + CHOICES_FOLDER_PATH
    
    create_interlocutors(spreadsheet_link, interlocutors_path, interlocutor_class)
    
    dialogue_values = gsd.download_google_sheet(spreadsheet_link, dialogue_sheet_name + DIALOGUES_RANGE)
    create_and_link_nodes(
        dialogue_sheet_name,
        container_class,
        single_node_class,
        choice_class,
        choice_node_class,
        choices_path,
        dialogue_values,
        destination_path,
        interlocutors_path,
        nodes_path
    )


def create_and_link_nodes(container_name, container_class, single_node_class, choice_class, choice_node_class, choices_path, dialogue_values, container_path, interlocutors_path, nodes_path):
    if try_create_dialogue_nodes(container_name, dialogue_values, container_class, single_node_class, choice_node_class, choice_class, container_path, nodes_path, choices_path, interlocutors_path):
        link_nodes(container_name, dialogue_values, container_path, nodes_path, choices_path)


def try_create_dialogue_nodes(container_name, dialogue_values, container_class, single_node_class, choice_node_class, choice_class, container_path, nodes_path, choices_path, interlocutors_path):    
    container_asset = af.create_or_get_data_asset(
        container_name,
        container_path,
        container_class
    )
    
    if container_asset is None:
        unreal.log_error(f"Container '{container_name}' could not be found or created.")
        return False
    
    for row in dialogue_values:
        node_asset_name = row[0]
        node_type = row[1]
        interlocutor_asset_name = row[2]
        sentence_sound_share_link = row[3]
        sentence_text = row[4]

        if node_type == "SingleNode":
            create_single_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, single_node_class, nodes_path, interlocutors_path)
        elif node_type == "ChoiceNode":
            create_choice_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, choice_node_class, choice_class, row, nodes_path, choices_path, interlocutors_path)

    return True

def link_nodes(container_name, dialogue_values, container_path, nodes_path, choices_path):
    container_full_path = container_path + f"/{container_name}"
    for row in dialogue_values:
        node_asset_name = row[0]
        node_type = row[1]
        
        try:
            next_node_asset_name = row[5]
        except IndexError:
            next_node_asset_name = None

        if node_type == "SingleNode":
            link_single_node(node_asset_name, next_node_asset_name,  nodes_path)
        elif node_type == "ChoiceNode":
            link_choice_node(node_asset_name, nodes_path, choices_path, row)

    first_node_asset_name = dialogue_values[0][0]
    if not unreal.EditorAssetLibrary.does_asset_exist(nodes_path + f"/{first_node_asset_name}"):
        unreal.log_error(f"First Node '{first_node_asset_name}' could not be found.")
        return None

    first_node_full_path = nodes_path + f"/{first_node_asset_name}"
    first_node = unreal.load_asset(first_node_full_path)
    container_asset = unreal.load_asset(container_full_path)
    container_asset.set_editor_property("StarterNode", first_node)
    unreal.EditorAssetLibrary.save_asset(container_full_path)


def link_single_node(node_asset_name, next_node_asset_name, nodes_path):
    node_full_path = nodes_path + f"/{node_asset_name}"
    next_node_full_path = nodes_path + f"/{next_node_asset_name}"

    if not unreal.EditorAssetLibrary.does_asset_exist(node_full_path):
        unreal.log_error(f"Node '{node_full_path}' could not be found.")
        return None

    single_node_asset = unreal.load_asset(node_full_path)

    if not unreal.EditorAssetLibrary.does_asset_exist(next_node_full_path):
        return None
        
    next_node_asset = unreal.load_asset(next_node_full_path)
    single_node_asset.set_editor_property("NextNode", next_node_asset)    
    unreal.EditorAssetLibrary.save_asset(node_full_path)


def link_choice_node(node_asset_name, nodes_path, choices_path, choice_node_row):
    choice_node_full_path = nodes_path + f"/{node_asset_name}"

    if not unreal.EditorAssetLibrary.does_asset_exist(choice_node_full_path):
        unreal.log_error(f"Choice Node '{choice_node_full_path}' could not be found.")
        return None

    choice_node = unreal.load_asset(choice_node_full_path)
    row = choice_node_row
    choice_index = 0
    for i in range(6, len(row), 2):
        choice_next_node = row[i + 1]
        choice_asset_name = f"{node_asset_name}_Choice_{choice_index}"
        choice_index += 1
        choice_full_path = choices_path + f"/{choice_asset_name}"
        choice_next_node_full_path = nodes_path + f"/{choice_next_node}"

        if not unreal.EditorAssetLibrary.does_asset_exist(choice_node_full_path):
            continue

        if not unreal.EditorAssetLibrary.does_asset_exist(choice_next_node_full_path):
            continue

        choice_asset = unreal.load_asset(choice_full_path)
        choice_next_node_asset = unreal.load_asset(choice_next_node_full_path)
        choice_asset.set_editor_property("NextNode", choice_next_node_asset)
        unreal.EditorAssetLibrary.save_asset(choice_full_path)

    return choice_node


def create_single_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, single_node_class, nodes_path, interlocutors_path):
    return create_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, single_node_class, nodes_path, interlocutors_path)


def create_choice_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, choice_node_class, choice_class, choice_node_row, nodes_path, choices_path, interlocutors_path):
    full_path = nodes_path + f"/{node_asset_name}"
    choice_node = create_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, choice_node_class, nodes_path, interlocutors_path)

    choices_assets = []
    row = choice_node_row
    choice_index = 0
    for i in range(6, len(row), 2):
        choice_text = row[i]

        choice_asset_name = f"{node_asset_name}_Choice_{choice_index}"
        choice_index += 1
        choice_asset = af.create_or_get_data_asset(
            choice_asset_name,
            choices_path,
            choice_class
        )

        if choice_asset is None:
            unreal.log_error(f"Choice '{choice_asset_name}' could not be found or created.")
            continue

        choice_full_path = choices_path + f"/{choice_asset_name}"
        choice_asset.set_editor_property("ChoiceText", choice_text)
        unreal.EditorAssetLibrary.save_asset(choice_full_path)
        choices_assets.append(choice_asset)

    choice_node.set_editor_property("Choices", choices_assets)
    unreal.EditorAssetLibrary.save_asset(full_path)
    return choice_node


def create_node(node_asset_name, interlocutor_asset_name, sentence_text, sentence_sound_share_link, node_class, nodes_path, interlocutors_path):
    full_path = nodes_path + f"/{node_asset_name}"
    node_asset = af.create_or_get_data_asset(
        node_asset_name,
        nodes_path,
        node_class
    )
    
    if node_asset is None:
        unreal.log_error(f"Node '{full_path}' could not be found or created.")
        return None
    
    interlocutor_full_path = interlocutors_path + f"/{interlocutor_asset_name}"
    if unreal.EditorAssetLibrary.does_asset_exist(interlocutor_full_path):
        interlocutor_asset = unreal.load_asset(interlocutor_full_path)
        node_asset.set_editor_property("Interlocutor", interlocutor_asset)
    
    if sentence_sound_share_link != "":
        sounds_path = nodes_path + "/Sounds"
        sentence_sound = af.create_unreal_asset_from_drive(
            f"S_{gih.get_id(sentence_sound_share_link)}",
            sounds_path,
            sentence_sound_share_link,
            unreal.SoundFactory(),
            "mp3"
        )
        node_asset.set_editor_property("SentenceSound", sentence_sound)

    node_asset.set_editor_property("Sentence", sentence_text)
    unreal.EditorAssetLibrary.save_asset(full_path)
    return node_asset


def create_interlocutors(spreadsheet_link, interlocutors_destination_path, interlocutor_asset_class):
    values = gsd.download_google_sheet(spreadsheet_link, INTERLOCUTORS_SHEET_RANGE)
    images_path = interlocutors_destination_path + "/Images"

    for row in values:
        asset_name = row[0]
        interlocutor_name = row[1]
        interlocutor_image_share_link = row[2]
        full_path = interlocutors_destination_path + f"/{asset_name}"

        interlocutor_asset = af.create_or_get_data_asset(
            asset_name,
            interlocutors_destination_path,
            interlocutor_asset_class
        )

        if interlocutor_asset is None:
            unreal.log_error(f"Interlocutor '{full_path}' could not be found or created.")
            continue
        
        if interlocutor_image_share_link != "":
            texture = af.create_unreal_asset_from_drive(
                f"T_{gih.get_id(interlocutor_image_share_link)}",
                images_path,
                interlocutor_image_share_link,
                unreal.TextureFactory(),
                "png"
            )
            interlocutor_asset.set_editor_property("Image", texture)

        interlocutor_asset.set_editor_property("Name", interlocutor_name)
        unreal.EditorAssetLibrary.save_asset(full_path)
