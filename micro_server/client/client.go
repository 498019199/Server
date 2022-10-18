package client

import (
	"micro_server/rpc"
	"micro_server/server"
	"sync"
)

type Client struct {
	cc      rpc.Proxy
	opt     *server.Option
	sending sync.Mutex
	header  rpc.Header
	mu      sync.Mutex
	seq     uint64
	pending map[uint64]*Call
}
