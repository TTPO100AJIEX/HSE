from hypothesis import given, strategies as st, settings, Verbosity
import math
import torch
import torchvision


@given(
    st.tuples(
        st.integers(min_value = 1, max_value = 8),
        st.integers(min_value = 1, max_value = 256),
        st.integers(min_value = 1, max_value = 4),
        st.booleans(),
        st.booleans(),
        st.lists(
            st.floats(allow_nan = False, allow_infinity = False), min_size = 8 * 1, max_size = 4098 * 4098
        ),
    )
)
@settings(verbosity = Verbosity.verbose, max_examples = 100000)
def test_lstm_forward(input):
    input_size, hidden_size, num_layers, bias, bidirectional, data = input
    device = torch.device('cuda')

    data = data[:math.floor(len(data) / input_size) * input_size]
    data = torch.tensor(data).reshape(-1, input_size).to(device)

    with torch.no_grad():
        lstm = torch.nn.LSTM(
            input_size = input_size,
            hidden_size = hidden_size,
            num_layers = num_layers,
            bias = bias,
            bidirectional = bidirectional,
            device = device
        ).eval()
        lstm(data)


@given(
    st.tuples(
        st.integers(min_value = 1, max_value = 8),
        st.integers(min_value = 1, max_value = 256),
        st.integers(min_value = 1, max_value = 4),
        st.booleans(),
        st.booleans(),
        st.lists(
            st.floats(allow_nan = False, allow_infinity = False), min_size = 8 * 1, max_size = 4098 * 4098
        ),
    )
)
@settings(verbosity = Verbosity.verbose, max_examples = 100000)
def test_lstm_backward(input):
    input_size, hidden_size, num_layers, bias, bidirectional, data = input
    device = torch.device('cuda')

    data = data[:math.floor(len(data) / input_size) * input_size]
    data = torch.tensor(data).reshape(-1, input_size).to(device)

    lstm = torch.nn.LSTM(
        input_size = input_size,
        hidden_size = hidden_size,
        num_layers = num_layers,
        bias = bias,
        bidirectional = bidirectional,
        device = device
    ).train()
    input, (h_n, c_n) = lstm(data)
    (input.mean() + h_n.mean() + c_n.mean()).backward()


@given(
    st.lists(
        st.floats(allow_nan = False, allow_infinity = False),
        min_size = 3 * 1 * 1, max_size = 10 * 4098 * 4098
    )
)
@settings(verbosity = Verbosity.verbose, max_examples = 10000)
def test_transforms(input):
    width = round(math.sqrt(len(input) // 3))
    height = len(input) // (3 * width)
    data = input[:(3 * width * height)]
    data = torch.tensor(data).reshape(3, width, height)

    weights = torchvision.models.get_model_weights('efficientnet_b7').DEFAULT
    weights.transforms()(data)

if __name__ == "__main__":
    test_lstm_forward()
    test_lstm_backward()
    test_transforms()