async function getStats() {
    try {
        const response = await fetch('/api/stats'); // Call Flask API endpoint
        const data = await response.json(); // Parse the JSON response

        if (data.error) {
            console.error("API Error:", data.error);
            // Update UI to show error
            document.getElementById('load-1min').innerText = 'Error';
            document.getElementById('mem-total').innerText = 'Error';
            return;
        }

        // Update CPU Load Averages
        document.getElementById('load-1min').innerText = data.load1.toFixed(2);
        document.getElementById('load-5min').innerText = data.load5.toFixed(2);
        document.getElementById('load-15min').innerText = data.load15.toFixed(2);

        // Update Memory Usage
        document.getElementById('mem-total').innerText = data.memtotal;
        document.getElementById('mem-free').innerText = data.memfree;

    } catch (error) {
        console.error("Failed to fetch stats:", error);
    }
}

getStats(); // Call once immediately to load data on page load
setInterval(getStats, 2000); // Call every 2 seconds for real-time updates but it will be updated for sure
