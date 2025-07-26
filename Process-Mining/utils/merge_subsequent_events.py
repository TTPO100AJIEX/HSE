import tqdm
import pandas

def merge_subsequent_events(event_log: pandas.DataFrame) -> pandas.DataFrame:
    event_log = event_log.copy()
    event_log_list = []
    for row in tqdm.tqdm(event_log.iterrows(), total = len(event_log)):
        event_log_list.append(row[1].to_dict())

    log_list_new = []
    for i in tqdm.trange(len(event_log_list) - 1):
        cur_event = event_log_list[i]
        next_event = event_log_list[i + 1]
        if (cur_event['case:concept:name'] == next_event['case:concept:name']) and (cur_event['concept:name'] == next_event['concept:name']):
            i += 1
            continue
        log_list_new.append(cur_event)
    log_list_new.append(event_log_list[-1])
    return pandas.DataFrame(log_list_new)
