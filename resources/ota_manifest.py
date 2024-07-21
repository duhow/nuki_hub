import re, json, argparse
from datetime import datetime, timezone

regex = r"\#define NUKI_HUB_VERSION \"(.*)\""
version = "unknown"

with open('src/Config.h', 'r') as file:
    file_content = file.read()
    matches = re.finditer(regex, file_content, re.MULTILINE)

    for matchNum, match in enumerate(matches, start=1):
        for groupNum in range(0, len(match.groups())):
            groupNum = groupNum + 1
            version = match.group(groupNum)

parser = argparse.ArgumentParser()

parser.add_argument('ota_type', type=str)
parser.add_argument('build', type=str)
args = parser.parse_args()

with open('ota/manifest.json', 'r+') as json_file:
    data = json.load(json_file)
    if (args.build == 'none'):
        data[args.ota_type]['time'] = "0000-00-00 00:00:00"
        data[args.ota_type]['version'] = "No beta available"
        data[args.ota_type]['build'] = ""
    else:
        data[args.ota_type]['time'] = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")
        data[args.ota_type]['version'] = version
        data[args.ota_type]['build'] = args.build
    json_file.seek(0)
    json.dump(data, json_file, indent=4)
    json_file.truncate()