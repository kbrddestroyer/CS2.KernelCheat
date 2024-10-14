import json

from collections import UserDict
from constants import JSON_CONSTANTS


class JsonParser(UserDict):
  g_json = {}

  def __init__(self, name: str, **kwargs) -> None:
    super(JsonParser, self).__init__(kwargs)
    with open(JSON_CONSTANTS.JSON_PATH + name) as f:
      self.data = json.load(f)
    JsonParser.g_json[name] = self
  

def get_json(name: str) -> JsonParser:
  if name not in JsonParser.g_json:
    JsonParser(name)
  return JsonParser.g_json[name]


def set_json(path: str) -> None:
  JSON_CONSTANTS.JSON_PATH = path