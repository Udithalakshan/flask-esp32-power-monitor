// Dashboard JavaScript for Plant Monitor
let chart;

async function fetchData() {
    try {
        const response = await fetch('/api/data');
        if (!response.ok) throw new Error('Failed to fetch data');
        
        const result = await response.json();
        const data = result.data || result;
        
        if (data.length === 0) {
            document.getElementById('noData').style.display = 'block';
            document.getElementById('readingsTable').style.display = 'none';
            return;
        }
        
        document.getElementById('noData').style.display = 'none';
        document.getElementById('readingsTable').style.display = 'table';
        
        updateStatistics(data);
        updateChart(data);
        updateTable(data);
        
    } catch (error) {
        console.error('Error fetching data:', error);
        document.getElementById('tableBody').innerHTML = 
            '<tr><td colspan="3" class="text-center text-danger">Error loading data</td></tr>';
    }
}

function updateStatistics(data) {
    if (data.length === 0) return;
    
    const values = data.map(d => parseFloat(d.value));
    const current = values[values.length - 1];
    const max = Math.max(...values);
    const min = Math.min(...values);
    const avg = values.reduce((a, b) => a + b, 0) / values.length;
    
    document.getElementById('currentValue').textContent = current.toFixed(2);
    document.getElementById('maxValue').textContent = max.toFixed(2);
    document.getElementById('minValue').textContent = min.toFixed(2);
    document.getElementById('avgValue').textContent = avg.toFixed(2);
}

function updateChart(data) {
    const labels = data.map(d => {
        const date = new Date(d.timestamp);
        return date.toLocaleTimeString();
    });
    
    const values = data.map(d => parseFloat(d.value));
    
    if (!chart) {
        const ctx = document.getElementById('sensorChart').getContext('2d');
        chart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [{
                    label: 'Sensor Value',
                    data: values,
                    borderColor: 'rgb(102, 126, 234)',
                    backgroundColor: 'rgba(102, 126, 234, 0.1)',
                    borderWidth: 2,
                    tension: 0.4,
                    fill: true,
                    pointRadius: 3,
                    pointHoverRadius: 5
                }]
            },
            options: {
                responsive: true,
                maintainAspectRatio: true,
                plugins: {
                    legend: {
                        display: true,
                        position: 'top'
                    },
                    tooltip: {
                        mode: 'index',
                        intersect: false
                    }
                },
                scales: {
                    x: {
                        title: {
                            display: true,
                            text: 'Time'
                        },
                        grid: {
                            display: true,
                            color: 'rgba(0, 0, 0, 0.05)'
                        }
                    },
                    y: {
                        title: {
                            display: true,
                            text: 'Sensor Value'
                        },
                        beginAtZero: true,
                        grid: {
                            display: true,
                            color: 'rgba(0, 0, 0, 0.05)'
                        }
                    }
                },
                animation: {
                    duration: 750
                }
            }
        });
    } else {
        chart.data.labels = labels;
        chart.data.datasets[0].data = values;
        chart.update('none'); // Update without animation for smoother updates
    }
}

function updateTable(data) {
    const tableBody = document.getElementById('tableBody');
    tableBody.innerHTML = '';
    
    // Show only the last 20 readings
    const recentData = data.slice(-20).reverse();
    
    recentData.forEach(reading => {
        const row = document.createElement('tr');
        row.innerHTML = `
            <td>${reading.id}</td>
            <td>${reading.timestamp}</td>
            <td><strong>${parseFloat(reading.value).toFixed(2)}</strong></td>
        `;
        tableBody.appendChild(row);
    });
}

// Refresh button handler
document.getElementById('refreshBtn').addEventListener('click', function() {
    this.innerHTML = '<i class="bi bi-arrow-clockwise"></i> Refreshing...';
    this.disabled = true;
    
    fetchData().then(() => {
        this.innerHTML = '<i class="bi bi-arrow-clockwise"></i> Refresh Data';
        this.disabled = false;
    });
});

// Initial data fetch
fetchData();

// Auto-refresh every 5 seconds
setInterval(fetchData, 5000);