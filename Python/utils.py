from pathlib import Path

def get_project_root() -> Path:
    return Path(__file__).parent.parent

def get_data_path(filename):
    return str(get_project_root()).replace("\\", "/") + "/C/data/" + filename
print(get_data_path("example.txt"))