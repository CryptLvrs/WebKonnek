function formatUptime(seconds) {
    const days = Math.floor(seconds / 86400);
    const hours = Math.floor((seconds % 86400) / 3600);
    const minutes = Math.floor((seconds % 3600) / 60);
    return days + "d " + hours + "h " + minutes + "m";
}

function formatMemory(kb) {
    const mb = Math.round(kb / 1024);
    const gb = (kb / 1024 / 1024).toFixed(2);
    return mb + " MB (" + gb + " GB)";
}

function formatDisk(bytes) {
    const gb = (bytes / 1024 / 1024 / 1024).toFixed(1);
    return gb + " GB";
}

async function getStats() {
    try {
        const response = await fetch('/api/stats'); // Call Flask API endpoint
        const data = await response.json(); // Parse the JSON response

        if (data.error) {
            console.error("API Error:", data.error);
            document.getElementById('load-1min').innerText = 'Error';
            document.getElementById('mem-total').innerText = 'Error';
            document.getElementById('connection').innerText = 'Offline';
            document.getElementById('connection').className = 'connection-bad';
            return;
        }

        // Update CPU Load Averages
        document.getElementById('load-1min').innerText = data.load1.toFixed(2);
        document.getElementById('load-5min').innerText = data.load5.toFixed(2);
        document.getElementById('load-15min').innerText = data.load15.toFixed(2);

        // Update Memory Usage
        document.getElementById('mem-total').innerText = formatMemory(data.memtotal);
        document.getElementById('mem-free').innerText = formatMemory(data.memfree);

        const freePercent = Math.round((data.memfree / data.memtotal) * 100);
        const freeEl = document.getElementById('mem-free');
        if (freePercent <= 15) {
            freeEl.classList.add('low');
        } else {
            freeEl.classList.remove('low');
        }

        document.getElementById('uptime').innerText = formatUptime(data.uptime);

        document.getElementById('disk-total').innerText = formatDisk(data.disk_total);
        document.getElementById('disk-free').innerText = formatDisk(data.disk_free);

        const diskFreePercent = Math.round((data.disk_free / data.disk_total) * 100);
        const diskEl = document.getElementById('disk-free');
        if (diskFreePercent <= 15) {
            diskEl.classList.add('low');
        } else {
            diskEl.classList.remove('low');
        }

        const now = new Date();
        document.getElementById('last-updated').innerText = now.toLocaleTimeString();
        document.getElementById('connection').innerText = 'Connected';
        document.getElementById('connection').className = 'connection-ok';

    } catch (error) {
        console.error("Failed to fetch stats:", error);
        document.getElementById('connection').innerText = 'Offline';
        document.getElementById('connection').className = 'connection-bad';
    }
}

getStats(); // Call once immediately to load data on page load
setInterval(getStats, 5000); // Call every 5 seconds for real-time updates but it will be updated for sure
