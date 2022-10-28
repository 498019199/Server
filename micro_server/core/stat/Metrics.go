package stat

import (
	"os"
	"time"
)

type (
	// Writer interface wraps the Write method.
	Writer interface {
		Write(report *StatReport) error
	}

	// A StatReport is a stat report entry.
	StatReport struct {
		Name          string  `json:"name"`
		Timestamp     int64   `json:"tm"`
		Pid           int     `json:"pid"`
		ReqsPerSecond float32 `json:"qps"`
		Drops         int     `json:"drops"`
		Average       float32 `json:"avg"`
		Median        float32 `json:"med"`
		Top90th       float32 `json:"t90"`
		Top99th       float32 `json:"t99"`
		Top99p9th     float32 `json:"t99p9"`
	}

	// A Metrics is used to log and report stat reports.
	Metrics struct {
		//executor  *executors.PeriodicalExecutor
		container *metricsContainer
	}
)

// NewMetrics returns a Metrics.
func NewMetrics(name string) *Metrics {
	container := &metricsContainer{
		name: name,
		pid:  os.Getpid(),
	}

	return &Metrics{
		//executor:  executors.NewPeriodicalExecutor(logInterval, container),
		container: container,
	}
}

// SetName sets the name of m.
func (m *Metrics) SetName(name string) {
	//m.executor.Sync(func() {
	m.container.name = name
	//})
}

type (
	tasksDurationPair struct {
		//tasks    []Task
		duration time.Duration
		drops    int
	}

	metricsContainer struct {
		name string
		pid  int
		//tasks    []Task
		duration time.Duration
		drops    int
	}
)
