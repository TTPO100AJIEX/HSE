import pm4py
import numpy

if __name__ == '__main__':
    log = pm4py.read_xes("3_modified.xes")

    procfiler = log["concept:name"].str.startswith("Procfiler/")
    gc = log["concept:name"].str.startswith("GC/")
    print(f'Procfiler: {sum(procfiler)}, GC: {sum(gc)}')

    log_min = log[procfiler & ~gc]

    items, counts = numpy.unique(log_min["concept:name"], return_counts = True)

    log_min2 = log_min[log_min["concept:name"].isin(items[counts > 20000])]

    print('discover_bpmn_inductive')
    bpmn = pm4py.discover_bpmn_inductive(log_min2, noise_threshold = 0.5, multi_processing = True)

    print('write_bpmn')
    pm4py.write_bpmn(bpmn, "bpmn.bpmn")