export default function Home() {
  return (
    <main className="min-h-screen bg-zinc-950 p-8 text-white">
      <div className="mx-auto max-w-7xl">
        {/* Header */}
        <header className="mb-8 flex items-center justify-between">
          <div>
            <h1 className="text-3xl font-bold tracking-tight">
              CANDY CADET
            </h1>
            <p className="mt-1 text-sm text-zinc-400">
              Robotics Control Dashboard
            </p>
          </div>

          <div className="flex items-center gap-2 rounded-full border border-green-900 bg-green-950/50 px-4 py-2">
            <span className="h-2.5 w-2.5 rounded-full bg-green-500" />
            <span className="text-sm text-green-400">ONLINE</span>
          </div>
        </header>

        {/* System Status */}
        <section className="mb-6 grid gap-4 md:grid-cols-3">
          <StatusCard
            title="Raspberry Pi"
            value="Connected"
            detail="Pi 5"
          />

          <StatusCard
            title="ESP32"
            value="Disconnected"
            detail="Firmware"
          />

          <StatusCard
            title="Battery"
            value="--"
            detail="Voltage"
          />
        </section>

        {/* Main Dashboard */}
        <section className="grid gap-6 lg:grid-cols-3">
          {/* Camera */}
          <div className="rounded-xl border border-zinc-800 bg-zinc-900 p-5 lg:col-span-2">
            <h2 className="mb-4 text-lg font-semibold">Camera</h2>

            <div className="flex aspect-video items-center justify-center rounded-lg border border-dashed border-zinc-700 bg-zinc-950">
              <div className="text-center">
                <p className="text-zinc-400">Camera feed unavailable</p>
                <p className="mt-1 text-xs text-zinc-600">
                  OpenCV camera stream will appear here
                </p>
              </div>
            </div>
          </div>

          {/* Robot State */}
          <div className="rounded-xl border border-zinc-800 bg-zinc-900 p-5">
            <h2 className="mb-4 text-lg font-semibold">Robot State</h2>

            <div className="space-y-4">
              <Telemetry label="Balance" value="--°" />
              <Telemetry label="Left Motor" value="--%" />
              <Telemetry label="Right Motor" value="--%" />
              <Telemetry label="CPU Temperature" value="--°C" />
              <Telemetry label="Uptime" value="--" />
            </div>
          </div>
        </section>

        {/* Scheduler */}
        <section className="mt-6 rounded-xl border border-zinc-800 bg-zinc-900 p-5">
          <div className="mb-4 flex items-center justify-between">
            <div>
              <h2 className="text-lg font-semibold">Scheduler</h2>
              <p className="text-xs text-zinc-500">
                Periodic firmware tasks
              </p>
            </div>

            <span className="rounded-full border border-green-900 bg-green-950/50 px-3 py-1 text-xs text-green-400">
              RUNNING
            </span>
          </div>

          <div className="overflow-hidden rounded-lg border border-zinc-800">
            <div className="grid grid-cols-3 border-b border-zinc-800 bg-zinc-950 px-4 py-3 text-xs font-medium uppercase tracking-wider text-zinc-500">
              <span>Task</span>
              <span>Interval</span>
              <span>Status</span>
            </div>

            <SchedulerTask
              name="Heartbeat"
              interval="500 ms"
              status="Running" // replace with actual data from system later
            />

            <SchedulerTask
              name="Status"
              interval="1000 ms"
              status="Running" // replace with actual data from system later
            />

            <SchedulerTask
              name="Battery"
              interval="--"
              status="Pending"
            />

            <SchedulerTask
              name="Sensor"
              interval="--"
              status="Pending"
            />

            <SchedulerTask
              name="Servo"
              interval="--"
              status="Pending"
            />

            <SchedulerTask
              name="Communication"
              interval="--"
              status="Pending"
            />
          </div>
        </section>
        {/* System Logs */}
        <section className="mt-6 rounded-xl border border-zinc-800 bg-zinc-900 p-5">
          <h2 className="mb-4 text-lg font-semibold">System Log</h2>

          <div className="rounded-lg bg-black p-4 font-mono text-sm text-zinc-400">
            <p>&gt; Candy Cadet dashboard initialized</p>
            <p>&gt; Raspberry Pi connection: ONLINE</p>
            <p>&gt; ESP32 connection: WAITING</p>
            <p>&gt; Sensor telemetry: WAITING</p>
          </div>
        </section>
      </div>
    </main>
  );
}

function StatusCard({
  title,
  value,
  detail,
}: {
  title: string;
  value: string;
  detail: string;
}) {
  return (
    <div className="rounded-xl border border-zinc-800 bg-zinc-900 p-5">
      <p className="text-sm text-zinc-400">{title}</p>
      <p className="mt-2 text-xl font-semibold">{value}</p>
      <p className="mt-1 text-xs text-zinc-600">{detail}</p>
    </div>
  );
}

function Telemetry({
  label,
  value,
}: {
  label: string;
  value: string;
}) {
  return (
    <div className="flex items-center justify-between border-b border-zinc-800 pb-3">
      <span className="text-sm text-zinc-400">{label}</span>
      <span className="font-mono text-sm">{value}</span>
    </div>
  );
}

function SchedulerTask({
  name,
  interval,
  status,
}: {
  name: string;
  interval: string;
  status: "Running" | "Pending";
}) {
  const isRunning = status === "Running";

  return (
    <div className="grid grid-cols-3 border-b border-zinc-800 px-4 py-3 last:border-b-0">
      <span className="text-sm">{name}</span>

      <span className="font-mono text-sm text-zinc-400">
        {interval}
      </span>

      <span className="flex items-center gap-2 text-sm">
        <span
          className={`h-2 w-2 rounded-full ${
            isRunning ? "bg-green-500" : "bg-zinc-600"
          }`}
        />

        <span className={isRunning ? "text-green-400" : "text-zinc-500"}>
          {status}
        </span>
      </span>
    </div>
  );
}
