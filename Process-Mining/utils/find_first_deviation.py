import typing
import collections

import tqdm

def find_first_deviation(event_log_list: typing.List[dict]):
    traces = {}
    for event in tqdm.tqdm(event_log_list):
        traceid, text = event['case:concept:name'], event['concept:name']
        if traceid not in traces:
            traces[traceid] = [ ]
        traces[traceid].append(text)


    i = 0
    open_events = []
    while i < max(map(lambda events: len(events), traces.values())):
        texts, traceids = [], []
        for traceid, events in traces.items():
            texts.append(events[i])
            traceids.append(traceid)
        
        if len(set(texts)) != 1:
            print(f"Deviation: {dict(collections.Counter(texts))}")
            return { 'text': texts, 'traceids': traceids, 'open_events': open_events }

        text = texts[0]
        if "Call:" in text:
            pass
        elif "Start:" in text:
            open_events.append(text)
        else:
            assert "End:" in text
            assert open_events[-1][len("Start"):] == text[len("End"):], f"{open_events[-1]} != {text}"
            open_events.pop()
        i += 1
