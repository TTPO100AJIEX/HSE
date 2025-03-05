import numpy
import pytest
import skimage.data

import cvtda.topology

def make_rgb():
    return skimage.transform.resize(skimage.data.astronaut(), (32, 32))

def make_gray():
    return make_rgb()[:, :, 0]

NUM_FILTRATIONS = (8 + 16 + 1 + 1 + 1 + 2) * 4


def test_gray_reduced():
    input = numpy.array([ make_gray() ])
    output = cvtda.topology.FeatureExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, NUM_FILTRATIONS * 56 * 2 + 2 * 56 * 2 + 2889)
    
def test_rgb_reduced():
    input = numpy.array([ make_rgb() ])
    output = cvtda.topology.FeatureExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, NUM_FILTRATIONS * 56 * 2 * 4 + 2 * 56 * 2 * 4 + 2889 * 4 + 1019)
    
def test_batch():
    input = numpy.array([ make_gray(), make_gray() ])
    output = cvtda.topology.FeatureExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (2, NUM_FILTRATIONS * 56 * 2 + 2 * 56 * 2 + 2889)



def test_transform_before_fit():
    input = numpy.array([ make_gray() ])
    with pytest.raises(AssertionError):
        cvtda.topology.FeatureExtractor(n_jobs = 1).transform(input)

def test_dimensions_mismatch():
    input1 = numpy.array([ make_gray() ])
    extractor = cvtda.topology.FeatureExtractor(n_jobs = 1).fit(input1)
    
    input2 = numpy.array([ make_rgb() ])
    with pytest.raises(AssertionError):
        extractor.transform(input2)

@pytest.mark.parametrize(
    ['shape'],
    [
        pytest.param((2, 16, 16, 16, 3), id = 'too many dimensions'),
        pytest.param((2, 16), id = 'too few dimensions'),
        pytest.param((2, 16, 16, 4), id = 'too many channels'),
        pytest.param((2, 16, 16, 2), id = 'too few channels')
    ]
)
def test_weird_shape(shape):
    input1 = numpy.random.rand(*shape)
    with pytest.raises(AssertionError):
        cvtda.topology.FeatureExtractor(n_jobs = 1).fit(input1)
