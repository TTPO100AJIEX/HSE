import typing

import pandas

from .remove_call_contents import remove_call_contents

def remove_package(event_log_list: typing.List[dict], package: str):
    event_log = pandas.DataFrame(event_log_list)

    to_remove = event_log[
        (event_log["concept:name"].str.startswith(f"Start:{package}."))
        | (event_log["concept:name"] == f"Start:{package}")
    ]["concept:name"].unique()
    for i, event in enumerate(to_remove):
        print(f"{i + 1}/{len(to_remove)}: {event}")
        event_log_list = remove_call_contents(
            event_log_list, event[len("Start:"):], False
        )

    log_list_final = []
    for event in event_log_list:
        if (event["concept:name"].startswith(f"Call:{package}.")
            or event["concept:name"] == f"Call:{package}"):
            continue
        log_list_final.append(event)

    return log_list_final
