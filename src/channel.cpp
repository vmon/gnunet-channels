#include <gnunet_channels/service.h>
#include <gnunet_channels/channel.h>
#include "channel_impl.h"

using namespace std;
using namespace gnunet_channels;

Channel::Channel(Service& service)
    : Channel(service.cadet())
{
}

Channel::Channel(std::shared_ptr<Cadet> cadet)
    : _scheduler(cadet->scheduler())
    , _ios(cadet->get_io_service())
    , _impl(make_shared<ChannelImpl>(move(cadet)))
{
}

Channel::Channel(std::shared_ptr<ChannelImpl> impl)
    : _scheduler(impl->scheduler())
    , _ios(impl->get_io_service())
    , _impl(move(impl))
{
}

asio::io_service& Channel::get_io_service()
{
    return _ios;
}

void Channel::connect_impl( std::string target_id
                          , const std::string& shared_secret
                          , OnConnect h)
{
    _impl->connect(move(target_id), shared_secret, move(h));
}

void Channel::write_impl(vector<uint8_t> data, OnWrite on_write)
{
    _impl->send(move(data), move(on_write));
}

void Channel::receive_impl(vector<asio::mutable_buffer> bufs, OnReceive h)
{
    _impl->receive(move(bufs), move(h));
}

void Channel::set_handle(GNUNET_CADET_Channel* h)
{
    _impl->_channel = h;
}

Channel::~Channel()
{
    // Could have been moved from.
    if (_impl) _impl->close();
}
