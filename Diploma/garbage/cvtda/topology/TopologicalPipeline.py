
from .Pipeline import Pipeline

class TopologicalPipeline(Pipeline):
    def __init__(
        self,
        n_jobs: int = -1,
        reduced: bool = True,
        only_get_from_dump: bool = False,
        return_diagrams: bool = False,
        **kwargs
    ):
        super().__init__(
            n_jobs = n_jobs,
            reduced = reduced,
            only_get_from_dump = only_get_from_dump,
            return_diagrams = return_diagrams,
            **kwargs
        )

    def final_dump_name_(self, dump_name: typing.Optional[str] = None):
        return self.features_dump_(dump_name)
    
    def features_dump_(self, dump_name: typing.Optional[str]):
        return (None if dump_name is None else f"{dump_name}/features")
        