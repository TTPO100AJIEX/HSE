import numpy
import pytest
import skimage.data

import cvtda.topology

def make_rgb():
    return skimage.transform.resize(skimage.data.astronaut(), (32, 32))

def make_gray():
    return make_rgb()[:, :, 0]


def test_gray_reduced():
    input = numpy.array([ make_gray() ])
    output = cvtda.topology.GeometryExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, 2233)
    assert numpy.isnan(output).sum() == 0
    
def test_gray_full():
    input = numpy.array([ make_gray() ])
    output = cvtda.topology.GeometryExtractor(n_jobs = 1, reduced = False).fit_transform(input)
    assert output.shape == (1, 3633)
    assert numpy.isnan(output).sum() == 0
    
def test_rgb_reduced():
    input = numpy.array([ make_rgb() ])
    output = cvtda.topology.GeometryExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (1, 2233 * 4 + 1019)
    assert numpy.isnan(output).sum() == 0
    
def test_rgb_full():
    input = numpy.array([ make_rgb() ])
    output = cvtda.topology.GeometryExtractor(n_jobs = 1, reduced = False).fit_transform(input)
    assert output.shape == (1, 3633 * 6 + 1019)
    assert numpy.isnan(output).sum() == 0

def test_batch():
    input = numpy.array([ make_gray(), make_gray() ])
    output = cvtda.topology.GeometryExtractor(n_jobs = 1).fit_transform(input)
    assert output.shape == (2, 2233)
    assert numpy.isnan(output).sum() == 0

def test_constant_pixels():
    input = numpy.zeros((1, 32, 32, 3))
    output = cvtda.topology.GeometryExtractor(n_jobs = 1, reduced = False).fit_transform(input)
    assert output.shape == (1, 3633 * 6 + 1019)
    assert numpy.isnan(output).sum() == 0


def test_transform_before_fit():
    input = numpy.array([ make_gray() ])
    with pytest.raises(AssertionError):
        cvtda.topology.GeometryExtractor(n_jobs = 1).transform(input)

def test_dimensions_mismatch():
    input1 = numpy.array([ make_gray() ])
    extractor = cvtda.topology.GeometryExtractor(n_jobs = 1).fit(input1)
    
    input2 = numpy.array([ make_rgb() ])
    with pytest.raises(AssertionError):
        extractor.transform(input2)
