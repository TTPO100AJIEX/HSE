from ru.hse.IServer import IServer
from ru.hse.AccountManager import AccountManager
from ru.hse.IPasswordEncoder import IPasswordEncoder


def test_class():
    # The fields should be set to instances, not statically
    # And the memory should not be shared between static and instance fields
    assert not hasattr(AccountManager, "server")
    assert not hasattr(AccountManager, "activeAccounts")
    assert not hasattr(AccountManager, "passEncoder")

def test_init(account_manager):
    assert account_manager.activeAccounts == { }
    assert isinstance(account_manager.server, IServer)
    assert isinstance(account_manager.passEncoder, IPasswordEncoder)