package rpc

import (
	"bufio"
	"encoding/gob"
	"io"
	"log"
)

type GobProxy struct {
	conn io.ReadWriteCloser
	buf  *bufio.Writer
	dec  *gob.Decoder
	enc  *gob.Encoder
}

func NewGobProxy(conn io.ReadWriteCloser) Proxy {
	buf := bufio.NewWriter(conn)
	return &GobProxy{
		conn: conn,
		buf:  buf,
		dec:  gob.NewDecoder(conn),
		enc:  gob.NewEncoder(buf),
	}
}

func (c *GobProxy) ReadHeader(h *Header) error {
	return c.dec.Decode(h)
}

func (c *GobProxy) ReadBody(body interface{}) error {
	return c.dec.Decode(body)
}

func (c *GobProxy) Write(h *Header, body interface{}) (err error) {
	defer func() {
		_ = c.buf.Flush()
		if err != nil {
			_ = c.Close()
		}
	}()
	if err := c.enc.Encode(h); err != nil {
		log.Println("rpc codec: gob error encoding header:", err)
		return err
	}
	if err := c.enc.Encode(body); err != nil {
		log.Println("rpc codec: gob error encoding body:", err)
		return err
	}
	return nil
}

func (c *GobProxy) Close() error {
	return c.conn.Close()
}
