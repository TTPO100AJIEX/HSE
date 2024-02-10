import math

import pytest

import doub
from AdvSqrt import AdvSqrt

@pytest.fixture()
def adv_sqrt():
    return AdvSqrt()

def trivial_checker(sqrt, input):
    restored = (sqrt ** 2)
    if restored == input: return
    
    side = math.inf if restored < input else -math.inf
    second_sqrt = math.nextafter(sqrt, side)

    lower, upper = sorted([ sqrt, second_sqrt ])
    assert (lower ** 2) < input
    assert (upper ** 2) > input


@pytest.mark.parametrize(
    [ 'input', 'answer' ],
    [
        [ 0, 0 ], # Zero
        [ 0.25, 0.5 ], # Non-integer below 1
        [ 1, 1 ], # One
        [ 2.25, 1.5 ], # Non-integer above 1
        [ 9, 3 ] # Integer above 1
    ]
)
def test_precise(adv_sqrt, input, answer):
    result = adv_sqrt.sqrt(input)
    assert result == answer


@pytest.mark.parametrize(
    [ 'input', 'expected_rounding' ],
    [ [ 3, 'down' ], [ 4, 'no' ], [ 5, 'up' ] ]
)
def test_rounding(adv_sqrt, input, expected_rounding):
    result = adv_sqrt.sqrt(input)
    trivial_checker(result, input)
    match expected_rounding:
        case 'down': assert result ** 2 < input
        case 'no': assert result ** 2 == input
        case 'up': assert result ** 2 > input
        case _: raise ValueError(f"Unexpected value of expected_rounding: {expected_rounding}")


@pytest.mark.parametrize(
    [ 'input', 'answer' ],
    [
        [ -100, 'NaN' ], # Negative
        [ doub.NEGATIVE_INFINITY, 'NaN' ], # -inf
        [ doub.POSITIVE_INFINITY, doub.POSITIVE_INFINITY ], # +inf
        [ doub.NEGATIVE_ZERO, doub.NEGATIVE_ZERO ], # -0
        [ 0, 0 ], # +0
        [ doub.NAN, 'NaN' ], # NaN
        [ doub.longBitsToDouble(0x7ff8000000000001), 'NaN' ], # NaN
        [ 4, 'Trivial' ], # Normalized
        [ doub.longBitsToDouble(1 << 20), 'Trivial' ], # Denormalized
        [ doub.MAX_VALUE, doub.longBitsToDouble(6913025428013711359) ], # Max Normal
        [ doub.MIN_NORMAL, 'Trivial' ], # Min Normal
        [ math.nextafter(doub.MIN_NORMAL, -math.inf), 'Trivial' ], # Max Subnormal
        [ doub.MIN_VALUE, 'Trivial' ] # Min Subnormal
    ]
)
def test_special(adv_sqrt, input, answer):
    result = adv_sqrt.sqrt(input)
    match answer:
        case 'NaN': assert doub.isnan(result)
        case 'Trivial': trivial_checker(result, input)
        case _: assert result == answer
