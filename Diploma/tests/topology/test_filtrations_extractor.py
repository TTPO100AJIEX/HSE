import numpy
import pytest
import skimage.data

import cvtda.topology

def make_rgb():
    return skimage.transform.resize(skimage.data.astronaut(), (32, 32))

def make_gray():
    return make_rgb()[:, :, 0]

NUM_FILTRATIONS_REDUCED = (8 + 16) * 3
NUM_FILTRATIONS_FULL = (8 + 16 + 1 + 1 + 1 + 2) * 9


def test_number_of_filtrations_reduced():
    extractor = cvtda.topology.FiltrationsExtractor(n_jobs = 1).fit(numpy.array([ make_gray() ]))
    assert len(extractor.filtration_extractors_) == NUM_FILTRATIONS_REDUCED
    
def test_number_of_filtrations_full():
    extractor = cvtda.topology.FiltrationsExtractor(n_jobs = 1, reduced = False).fit(numpy.array([ make_gray() ]))
    assert len(extractor.filtration_extractors_) == NUM_FILTRATIONS_FULL

def test_gray_reduced():
    input = numpy.array([ make_gray() ])
    output = cvtda.topology.FiltrationsExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, NUM_FILTRATIONS_REDUCED * 56 * 2)
    assert numpy.isnan(output).sum() == 0

def test_gray_full():
    input = numpy.array([ make_gray() ])
    output = cvtda.topology.FiltrationsExtractor(n_jobs = 1, reduced = False).fit_transform(input)
    assert output.shape == (1, NUM_FILTRATIONS_FULL * 126 * 2)
    assert numpy.isnan(output).sum() == 0
    
def test_rgb_reduced():
    input = numpy.array([ make_rgb() ])
    output = cvtda.topology.FiltrationsExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, NUM_FILTRATIONS_REDUCED * 56 * 2 * 4)
    assert numpy.isnan(output).sum() == 0

def test_batch():
    input = numpy.array([ make_gray(), make_gray() ])
    output = cvtda.topology.FiltrationsExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (2, NUM_FILTRATIONS_REDUCED * 56 * 2)
    assert numpy.isnan(output).sum() == 0



def test_transform_before_fit():
    input = numpy.array([ make_gray() ])
    with pytest.raises(AssertionError):
        cvtda.topology.FiltrationsExtractor(n_jobs = 1).transform(input)

def test_dimensions_mismatch():
    input1 = numpy.array([ make_gray() ])
    extractor = cvtda.topology.FiltrationsExtractor(n_jobs = 1).fit(input1)
    
    input2 = numpy.array([ make_rgb() ])
    with pytest.raises(AssertionError):
        extractor.transform(input2)
