import torch
import torchvision.models

def get_model():
    # https://github.com/pytorch/vision/issues/7744
    def get_state_dict(self, *args, **kwargs):
        kwargs.pop("check_hash")
        return torch.hub.load_state_dict_from_url(self.url, *args, **kwargs)
    torchvision.models._api.WeightsEnum.get_state_dict = get_state_dict

    weights = torchvision.models.get_model_weights("efficientnet_b3").DEFAULT
    return torchvision.models.get_model("efficientnet_b3", weights = weights).eval()