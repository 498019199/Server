package rpc

import "io"

type Header struct {
	ServiceMothod string
	Seq           int64
	Error         string
}

type Rpc interface {
	io.Closer
	ReadHeader(header *Header) error
	ReadBoy(interface{ interface{} }) error
	Write(*Header, interface{}) error
}
