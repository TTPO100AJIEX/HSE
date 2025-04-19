import numpy
import pytest
import gtda.images
import skimage.data

import cvtda.topology

def make_rgb():
    return skimage.transform.resize(skimage.data.astronaut(), (32, 32))

def make_gray():
    return make_rgb()[:, :, 0]

def make_filtration_extractor(reduced: bool):
    return cvtda.topology.FiltrationExtractor(
        gtda.images.HeightFiltration,
        { 'direction': numpy.array([ 1, 1 ]) },
        0.4,
        n_jobs = 1,
        reduced = reduced
    )


def test_gray_reduced():
    input = numpy.array([ make_gray() ])
    output = make_filtration_extractor(reduced = True).fit_transform(input)
    assert output.shape == (1, 56 * 2)
    assert numpy.isnan(output).sum() == 0
    
def test_gray_full():
    input = numpy.array([ make_gray() ])
    output = make_filtration_extractor(reduced = False).fit_transform(input)
    assert output.shape == (1, 126 * 2)
    assert numpy.isnan(output).sum() == 0
    
def test_rgb_reduced():
    input = numpy.array([ make_rgb() ])
    output = make_filtration_extractor(reduced = True).fit_transform(input)
    assert output.shape == (1, 56 * 2 * 4)
    assert numpy.isnan(output).sum() == 0
    
def test_rgb_full():
    input = numpy.array([ make_rgb() ])
    output = make_filtration_extractor(reduced = False).fit_transform(input)
    assert output.shape == (1, 126 * 2 * 6)
    assert numpy.isnan(output).sum() == 0

def test_batch():
    input = numpy.array([ make_gray(), make_gray() ])
    output = make_filtration_extractor(reduced = True).fit_transform(input)
    assert output.shape == (2, 56 * 2)
    assert numpy.isnan(output).sum() == 0



def test_transform_before_fit():
    input = numpy.array([ make_gray() ])
    with pytest.raises(AssertionError):
        make_filtration_extractor(reduced = True).transform(input)

def test_dimensions_mismatch():
    input1 = numpy.array([ make_gray() ])
    extractor = make_filtration_extractor(reduced = True).fit(input1)
    
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
        make_filtration_extractor(reduced = True).fit(input1)
