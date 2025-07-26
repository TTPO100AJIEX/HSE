import tqdm
import pandas

def merge_paired_events(event_log: pandas.DataFrame) -> pandas.DataFrame:
    event_log = event_log.copy()
    event_log_list = []
    for row in tqdm.tqdm(event_log.iterrows(), total = len(event_log)):
        event_log_list.append(row[1].to_dict())


    edges_in, edges_out = {}, {}
    for i in tqdm.trange(len(event_log_list) - 1):
        if event_log_list[i]["case:concept:name"] != event_log_list[i + 1]["case:concept:name"]:
            continue

        cur_text = event_log_list[i]["concept:name"]
        next_text = event_log_list[i + 1]["concept:name"]

        if cur_text not in edges_out:
            edges_out[cur_text] = set()
        edges_out[cur_text].add(next_text)

        if next_text not in edges_in:
            edges_in[next_text] = set()
        edges_in[next_text].add(cur_text)
        
        if cur_text not in edges_in:
            edges_in[cur_text] = set()
        if next_text not in edges_out:
            edges_out[next_text] = set()


    edges_in_count = {}
    for key, value in edges_in.items():
        edges_in_count[key] = len(value)
    
    edges_out_count = {}
    for key, value in edges_out.items():
        edges_out_count[key] = len(value)

    i = 0
    pbar = tqdm.trange(0, len(event_log_list))
    pbar_iter = iter(pbar)
    while i < len(event_log_list) - 1:
        cur_event = event_log_list[i]
        next_event = event_log_list[i + 1]
        if cur_event["case:concept:name"] != next_event["case:concept:name"]:
            i += 1
            next(pbar_iter)
            continue

        exactly_one_out = (len(edges_out[cur_event['concept:name']]) == 1)
        exactly_one_in = (len(edges_in[next_event['concept:name']]) == 1)

        if not exactly_one_out or not exactly_one_in:
            i += 1
            next(pbar_iter)
            continue

        new_name = cur_event['concept:name'] + "\n" + next_event['concept:name']

        if new_name not in edges_in:
            edges_in[new_name] = set()
        edges_in[new_name] = edges_in[new_name].union(edges_in[cur_event['concept:name']])
        
        if new_name not in edges_out:
            edges_out[new_name] = set()
        edges_out[new_name] = edges_out[new_name].union(edges_out[next_event['concept:name']])

        event_log_list[i]['concept:name'] = new_name
        del event_log_list[i + 1]
        pbar.total = len(event_log_list)

    return pandas.DataFrame(event_log_list)
