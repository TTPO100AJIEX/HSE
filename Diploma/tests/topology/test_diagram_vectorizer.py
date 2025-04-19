import numpy
import pytest

import cvtda.topology

INPUT = numpy.array([[
    [ 0., 1, 1 ],
    [ 0, 2, 1 ],
    [ 1, 3, 1 ]
]])

def test_reduced():
    output = cvtda.topology.DiagramVectorizer(n_jobs = 1).fit_transform(INPUT)
    assert output.shape == (1, 56)

def test_full():
    output = cvtda.topology.DiagramVectorizer(reduced = False, n_jobs = 1).fit_transform(INPUT)
    assert output.shape == (1, 126)



def test_transform_before_fit():
    with pytest.raises(AssertionError):
        cvtda.topology.DiagramVectorizer(n_jobs = 1).transform(INPUT)
