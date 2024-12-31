import numpy
import pytest

import cvtda.topology


def test_fit_transform():
    greyscale_to_pointclouds = cvtda.topology.GreyscaleToPointClouds()
    output = greyscale_to_pointclouds.fit_transform(numpy.random.rand(10, 28, 28))
    assert len(output) == 10
    assert numpy.all([
        len(item) == 4
        and item[0].shape[1] == 3 and item[1].shape[1] == 3
        and item[2].shape[1] == 2 and item[3].shape[1] == 2
        for item in output
    ])


def test_handmade_pointcloud():
    input = [
        [ 0,  255, 0, 0   ],
        [ 0,  0,   0, 0   ],
        [ 0,  101,   0, 240 ],
        [ 10, 0,   76, 0   ],
    ]
    expected_output = numpy.array([
        [ 1, 1, 101 ],
        [ 1, 3, 255 ],
        [ 2, 0, 76 ],
        [ 3, 1, 240 ]
    ])

    greyscale_to_pointclouds = cvtda.topology.GreyscaleToPointClouds()
    output = greyscale_to_pointclouds.fit_transform(numpy.array([ input ]))
    assert numpy.all(output[0][0] == expected_output)


def test_transform_before_fit():
    greyscale_to_pointclouds = cvtda.topology.GreyscaleToPointClouds()
    with pytest.raises(AssertionError):
        greyscale_to_pointclouds.transform(numpy.random.rand(10, 28, 28))
