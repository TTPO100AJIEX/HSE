import numpy
import pytest

import cvtda.utils


def numpy_data():
    data1 = [
        [ [ 1, 2, 3, 4 ], [ 5, 6, 7, 8 ] ],
        [ [ 9, 8, 7, 6 ], [ 5, 4, 3, 2 ] ]
    ]
    data2 = [
        [ [ 2, 4, 6, 8 ], [ 1, 3, 5, 7 ] ],
        [ [ 8, 4, 6, 2 ], [ 7, 3, 5, 1 ] ]
    ]
    data3 = [
        [ [ 0, 9, 1, 8 ], [ 2, 7, 3, 6 ] ],
        [ [ 4, 5, 5, 4 ], [ 5, 4, 4, 5 ] ]
    ]
    return {
        'input': numpy.array([ data1, data2, data3 ]),
        'flat': numpy.array([ *data1, *data2, *data3 ]),
    }

def list_data():
    data1 = [
        [ [ 1, 2 ], [ 5, 6, 7, 8 ] ],
        [ 5, 4, 3, 2 ]
    ]
    data2 = [
        [ [ 2 ], 7 ],
        [ [ 8, 4, 6, 2 ], 7, 3, 5, 1 ]
    ]
    data3 = [
        [ [ 0, 9, 8 ], [ 2, 6 ] ],
        [ ]
    ]
    return {
        'input': [ data1, data2, data3 ],
        'flat': [ *data1, *data2, *data3 ],
    }

def assert_iterables_equal(lhs, rhs):
    if hasattr(rhs, '__len__'):
        assert len(lhs) == len(rhs)
        map(assert_iterables_equal, zip(lhs, rhs))
    else:
        assert lhs == rhs


@pytest.mark.parametrize(
    ['fit_data'],
    [
        pytest.param(numpy_data(), id = 'fit - numpy'),
        pytest.param(list_data(), id = 'fit - list')
    ]
)
@pytest.mark.parametrize(
    ['transform_data'],
    [
        pytest.param(numpy_data(), id = 'transform - numpy'),
        pytest.param(list_data(), id = 'transform - list')
    ]
)
@pytest.mark.parametrize(
    ['inverse_transform_data'],
    [
        pytest.param(numpy_data(), id = 'inverse_transform - numpy'),
        pytest.param(list_data(), id = 'inverse_transform - list')
    ]
)
def test_there_and_back(fit_data, transform_data, inverse_transform_data):
    flatten_batch = cvtda.utils.flatten_batch().fit(fit_data['input'])
    assert_iterables_equal(
        flatten_batch.transform(transform_data['input']),
        transform_data['flat']
    )
    assert_iterables_equal(
        flatten_batch.inverse_transform(inverse_transform_data['flat']),
        inverse_transform_data['input']
    )


@pytest.mark.parametrize(
    ['data', 'expected_error'],
    [
        pytest.param(1, TypeError, id = 'not an array'),
        pytest.param([ 1, 2 ], TypeError, id = 'no dimensions'),
        pytest.param([ [ 1, 2 ], [ 1 ] ], AssertionError, id = 'inhomogeneous dimensions')
    ]
)
def test_fit_bad_data(data, expected_error):
    flatten_batch = cvtda.utils.flatten_batch()
    with pytest.raises(expected_error):
        flatten_batch.fit(data)


@pytest.mark.parametrize(
    ['data', 'expected_error'],
    [
        pytest.param(1, TypeError, id = 'not an array'),
        pytest.param([ 1, 2 ], TypeError, id = 'no dimensions'),
        pytest.param([ [ 1 ], [ 2 ] ], AssertionError, id = 'dimensions mismatch'),
        pytest.param([ [ 1, 2 ], [ 1 ] ], AssertionError, id = 'inhomogeneous dimensions')
    ]
)
def test_transform_bad_data(data, expected_error):
    flatten_batch = cvtda.utils.flatten_batch().fit(numpy_data()['input'])
    with pytest.raises(expected_error):
        flatten_batch.transform(data)


@pytest.mark.parametrize(
    ['data', 'expected_error'],
    [
        pytest.param(1, TypeError, id = 'not an array'),
        pytest.param([ 1, 2, 3 ], AssertionError, id = 'indivisible number of items')
    ]
)
def test_inverse_transform_bad_data(data, expected_error):
    flatten_batch = cvtda.utils.flatten_batch().fit(numpy_data()['input'])
    with pytest.raises(expected_error):
        flatten_batch.inverse_transform(data)


def test_transform_before_fit():
    flatten_batch = cvtda.utils.flatten_batch()
    with pytest.raises(AssertionError):
        flatten_batch.transform(numpy_data()['input'])


def test_inverse_transform_before_fit():
    flatten_batch = cvtda.utils.flatten_batch()
    with pytest.raises(AssertionError):
        flatten_batch.inverse_transform(numpy_data()['flat'])
