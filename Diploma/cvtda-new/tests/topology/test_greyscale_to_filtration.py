import numpy
import pytest

import cvtda.topology


def test_fit_transform():
    greyscale_to_filtrations = cvtda.topology.greyscale_to_filtrations(
        radial_filtration_centers = [ (5, 5), (10, 10), (15, 15) ]
    )
    output = greyscale_to_filtrations.fit_transform(numpy.random.rand(10, 28, 28))
    assert output.shape == (10, 35, 28, 28)


def test_transform_before_fit():
    greyscale_to_filtrations = cvtda.topology.greyscale_to_filtrations()
    with pytest.raises(AssertionError):
        greyscale_to_filtrations.transform(numpy.random.rand(10, 28, 28))
