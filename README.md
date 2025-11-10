# ⚡ Three-Phase Power Monitoring System

A complete real-time power monitoring solution that bridges industrial power analyzers with modern web interfaces using ESP32, Flask, and Modbus RTU protocol.

![System Architecture]([https://www.linkedin.com/posts/uditha-lakshan-520411278_powermonitoring-energydashboard-realtimeanalytics-activity-7390069767358955521-2VHl?utm_source=share&utm_medium=member_desktop&rcm=ACoAAEOxSqgBC7VCB9X1fRT96mgqiVseo-0vUdU])
*Real-time monitoring of three-phase electrical parameters with interactive charts*

## 🌟 Features

### 📊 Real-Time Monitoring
- **Three-Phase Parameters**: Track voltage, current, power factor, frequency, active/apparent power
- **Live Updates**: Data refreshes every 3 seconds automatically
- **6 Interactive Charts**: Phase voltages, phase currents, frequency, power factor, active power, apparent power
- **Fullscreen Mode**: Click any chart header to expand for detailed analysis
- **5-Minute Rolling Window**: Automatic old data filtering for optimal performance

### 📈 Data Visualization
- Color-coded phase identification (Phase A: Red, B: Orange, C: Blue)
- Real-time chart updates with Chart.js
- Smooth animations and responsive design
- Time-series graphs with proper axis scaling
- Mobile-friendly interface

### 📥 Data Export
- **One-Click CSV Download**: Export all readings with timestamp
- Includes 11 parameters: Timestamp, 3 Phase Voltages, 3 Phase Currents, Frequency, Power Factor, Active Power, Apparent Power
- ISO timestamp file naming for easy organization
- Perfect for analysis, reporting, and compliance

### 🔐 Security
- API key authentication for ESP32 devices
- User registration and login system
- Password hashing with Flask-Bcrypt
- Session management with Flask-Login
- Protected API endpoints

### 🚦 System Health
- Live connection status indicators
- Automatic stale data detection (5-minute threshold)
- Real-time last-update timestamp
- Error logging and diagnostics

## 🛠️ Technology Stack

### Backend
- **Python 3.8+**
- **Flask 3.0.3** - Web framework
- **SQLAlchemy 3.1.1** - ORM and database management
- **Flask-Login 0.6.3** - User session management
- **Flask-Bcrypt 1.0.1** - Password hashing
- **Flask-Migrate 4.0.7** - Database migrations
- **python-dotenv 1.0.1** - Environment variable management

### Frontend
- **HTML5, CSS3, JavaScript (ES6+)**
- **Bootstrap 5.3.3** - Responsive UI framework
- **Chart.js** - Interactive charts
- **Bootstrap Icons** - Icon library

### Hardware
- **ESP32 DevKit** - WiFi-enabled microcontroller
- **SDM630 Power Analyzer** - Three-phase energy meter
- **RS485 to TTL Module** - Modbus RTU communication
- **MAX485 Module** - RS485 transceiver

### Communication
- **Modbus RTU** - Industrial communication protocol
- **RESTful API** - JSON data exchange
- **WebSocket-ready** - For future real-time updates

## 📋 Prerequisites

### Software Requirements
- Python 3.8 or higher
- pip package manager
- Arduino IDE or PlatformIO (for ESP32 programming)
- Modern web browser (Chrome, Firefox, Edge, Safari)

### Hardware Requirements
- ESP32 development board
- SDM630 or compatible three-phase power analyzer
- RS485 to TTL converter module
- USB cable for ESP32 programming
- Power supply for ESP32 (5V/1A minimum)

### Libraries for ESP32
- ArduinoJson (6.x)
- ModbusMaster
- WiFi (ESP32 core)
- HTTPClient (ESP32 core)

## 🚀 Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Udithalakshan/three-phase-power-monitor.git
cd three-phase-power-monitor
```

### 2. Set Up Python Virtual Environment
```bash
# Create virtual environment
python -m venv env

# Activate virtual environment
# On Windows:
env\Scripts\activate
# On macOS/Linux:
source env/bin/activate
```

### 3. Install Python Dependencies
```bash
pip install -r requirements.txt
```

### 4. Configure Environment Variables
Create a `.env` file in the root directory:
```env
FLASK_APP=run.py
FLASK_DEBUG=True
FLASK_ENV=development
SECRET_KEY='your-secret-key-change-in-production'

# API Key for ESP32 (MUST match ESP32 code)
ESP32_API_KEY=yPowerAnalyzerKey456

# Database
DATABASE_URL=sqlite:///instance/power_analyzer.db
```

### 5. Initialize Database
```bash
python run.py
```
The database will be automatically created on first run.

### 6. Create User Account
Navigate to `http://127.0.0.1:5000/auth/register` and create your account.

### 7. Configure ESP32

#### Update WiFi Credentials
Edit the ESP32 code (`esp32_power_monitor.ino`):
```cpp
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
```

#### Update Flask Server IP
```cpp
const char* FLASK_SERVER_URL = "http://YOUR_PC_IP:5000/api/data";
const char* API_KEY = "yPowerAnalyzerKey456";  // Must match .env file
```

#### Find Your PC's IP Address
```bash
# Windows
ipconfig

# macOS/Linux
ifconfig
# or
ip addr show
```

### 8. Upload Code to ESP32
1. Open Arduino IDE or PlatformIO
2. Select board: ESP32 Dev Module
3. Select correct COM port
4. Upload the code
5. Open Serial Monitor (115200 baud) to verify connection

## 🔌 Hardware Wiring

### ESP32 to RS485 Module
```
ESP32          RS485 Module
-----          ------------
GPIO 17 (TX2)  → DI (Data In)
GPIO 16 (RX2)  → RO (Receive Out)
GPIO 4         → DE/RE (Driver Enable)
3.3V           → VCC
GND            → GND
```

### RS485 Module to SDM630
```
RS485 Module   SDM630
------------   ------
A              → A/+
B              → B/-
```

### Power Connections
- ESP32: 5V via USB or external power supply
- RS485 Module: 3.3V from ESP32
- SDM630: 230V AC (follow manufacturer's safety guidelines)

**⚠️ WARNING**: SDM630 operates at mains voltage. Installation should be performed by qualified electricians only.

## 🎯 Usage

### Starting the Flask Server
```bash
# Activate virtual environment
source env/bin/activate  # macOS/Linux
env\Scripts\activate     # Windows

# Run the application
python run.py
```

The server will start at `http://0.0.0.0:5000`

### Accessing the Dashboard
1. Open browser and navigate to `http://YOUR_PC_IP:5000`
2. Login with your credentials
3. View real-time power monitoring data
4. Click chart headers for fullscreen view
5. Use "Download Data List" to export CSV

### ESP32 Operation
Once configured and powered:
1. ESP32 connects to WiFi automatically
2. Reads data from SDM630 via Modbus RTU every 3 seconds
3. Sends data to Flask server via HTTP POST
4. Automatic reconnection on connection loss

### API Endpoints

#### POST `/api/data`
Submit power readings from ESP32
- **Authentication**: API Key in `X-API-Key` header
- **Content-Type**: `application/json`
- **Body**:
```json
{
  "voltage_a": 230.5,
  "voltage_b": 231.2,
  "voltage_c": 229.8,
  "voltage_ab": 399.1,
  "voltage_bc": 400.2,
  "voltage_ca": 398.5,
  "current_a": 1.45,
  "current_b": 1.52,
  "current_c": 1.38,
  "frequency": 50.02,
  "power_factor": 0.92,
  "active_power": 1050.5,
  "reactive_power": 425.2,
  "apparent_power": 1135.8,
  "voltage_thd_avg": 2.1,
  "current_thd_avg": 3.5
}
```

#### GET `/api/data`
Retrieve all power readings (requires login)
- **Authentication**: Session cookie
- **Response**: JSON array of readings

#### GET `/api/latest`
Get most recent reading (requires login)
- **Authentication**: Session cookie
- **Response**: Single reading object

#### GET `/api/status`
Check API health (public)
- **Response**: API status and endpoints

## 📁 Project Structure

```
three-phase-power-monitor/
├── project/
│   ├── __init__.py           # Flask app factory
│   ├── models.py             # Database models
│   ├── auth.py               # Authentication routes
│   ├── main.py               # Main routes
│   ├── api.py                # API endpoints
│   ├── templates/
│   │   ├── base.html         # Base template
│   │   ├── login.html        # Login page
│   │   ├── register.html     # Registration page
│   │   └── dashboard.html    # Main dashboard
│   └── static/               # Static files (if any)
├── instance/
│   └── power_analyzer.db     # SQLite database (auto-created)
├── esp32_power_monitor/
│   └── esp32_power_monitor.ino  # ESP32 Arduino code
├── config.py                 # Flask configuration
├── run.py                    # Application entry point
├── requirements.txt          # Python dependencies
├── .env                      # Environment variables
├── .gitignore               # Git ignore file
└── README.md                # This file
```

## 🔧 Configuration

### Database Configuration
By default, SQLite is used. For production, use PostgreSQL:

1. Install PostgreSQL
2. Create database: `createdb power_analyzer`
3. Update `.env`:
```env
DATABASE_URL=postgresql://username:password@localhost/power_analyzer
```

### Timezone Configuration
Default timezone is `Asia/Colombo`. To change, edit `models.py`:
```python
default=lambda: datetime.now(pytz.timezone('Your/Timezone')).replace(tzinfo=None)
```

### Data Retention
Currently shows last 5 minutes. To change, edit `dashboard.html`:
```javascript
const TIME_WINDOW_MINUTES = 10; // Change to desired minutes
```

### ESP32 Update Interval
Default is 3 seconds. To change, edit ESP32 code:
```cpp
const unsigned long SEND_INTERVAL = 5000; // milliseconds
```

## 📊 Dashboard Features

### Real-Time Charts
1. **Phase Voltages**: Line-to-Neutral voltages for all three phases
2. **Phase Currents**: Current consumption per phase
3. **Frequency**: System frequency monitoring (typically 50/60 Hz)
4. **Power Factor**: Power quality indicator (0-1 range)
5. **Active Power**: Real power consumption in Watts
6. **Apparent Power**: Total power demand in VA

### Fullscreen Mode
- Click any chart header to expand
- Real-time updates continue in fullscreen
- Press X or ESC to close

### Data Export
- Click "Download Data List" button
- CSV includes all parameters with timestamps
- File naming: `PowerAnalyzer_YYYY-MM-DDTHH-MM.csv`

## 🐛 Troubleshooting

### ESP32 Can't Connect to WiFi
- Verify SSID and password are correct
- Check WiFi signal strength
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Restart ESP32 and router

### "Connection Refused" Error
- Verify Flask server is running: `python run.py`
- Check PC's IP address hasn't changed
- Update `FLASK_SERVER_URL` in ESP32 code
- Disable Windows Firewall temporarily to test
- Ensure PC and ESP32 are on same network

### No Data in Dashboard
- Check ESP32 Serial Monitor for errors
- Verify API key matches between ESP32 and `.env`
- Test API endpoint: `http://YOUR_IP:5000/api/status`
- Check if user account exists
- Verify Modbus wiring (A to A, B to B)

### Modbus Communication Errors
- Check RS485 wiring (A/B may be swapped)
- Verify SDM630 slave address (default is 1)
- Test with Modbus testing software
- Check baud rate (default 9600)
- Ensure proper grounding

### Database Errors
- Delete `instance/power_analyzer.db` and restart
- Check file permissions
- Run `flask db upgrade` if using migrations

### Charts Not Updating
- Check browser console for JavaScript errors
- Clear browser cache
- Verify data is being received: check Network tab
- Ensure Flask server is running

## 🔒 Security Considerations

### Production Deployment
1. **Change SECRET_KEY**: Generate a strong random key
```python
import secrets
secrets.token_hex(32)
```

2. **Change API Key**: Use a strong, unique key
3. **Use HTTPS**: Set up SSL/TLS certificates
4. **Enable CORS**: Configure allowed origins
5. **Use Production Database**: PostgreSQL or MySQL
6. **Set DEBUG=False**: In production environment
7. **Use Gunicorn**: Replace Flask development server
```bash
gunicorn -w 4 -b 0.0.0.0:5000 run:app
```

8. **Add Rate Limiting**: Prevent API abuse
9. **Regular Backups**: Automate database backups
10. **Update Dependencies**: Keep libraries up to date

### Network Security
- Use VPN for remote access
- Implement firewall rules
- Disable unused ports
- Use strong WiFi passwords
- Enable MAC address filtering

## 📈 Performance Optimization

### Database Optimization
```sql
-- Add indexes for better query performance
CREATE INDEX idx_timestamp ON power_reading(timestamp DESC);
CREATE INDEX idx_user_id ON power_reading(user_id);
```

### Data Cleanup
Implement automatic data cleanup for old records:
```python
# Add to scheduled task
from datetime import datetime, timedelta
cutoff = datetime.now() - timedelta(days=30)
PowerReading.query.filter(PowerReading.timestamp < cutoff).delete()
db.session.commit()
```

### Caching
Consider implementing Redis for caching:
```python
from flask_caching import Cache
cache = Cache(app, config={'CACHE_TYPE': 'redis'})
```

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/YourFeature`
3. Commit changes: `git commit -m 'Add YourFeature'`
4. Push to branch: `git push origin feature/YourFeature`
5. Open a Pull Request

### Code Style
- Follow PEP 8 for Python code
- Use meaningful variable names
- Add comments for complex logic
- Update documentation for new features

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Your Name**
- GitHub: [@yourusername]([https://github.com/yourusername](https://github.com/UdithaLakshan))
- LinkedIn: [Your LinkedIn]([https://linkedin.com/in/yourprofile](https://www.linkedin.com/in/uditha-lakshan-520411278))
- Email: lakshankjhu@gmail.com

## 🙏 Acknowledgments

- Flask documentation and community
- Chart.js for excellent charting library
- Bootstrap team for responsive framework
- ESP32 community for hardware support
- SDM630 Modbus protocol documentation

## 📚 References

- [Flask Documentation](https://flask.palletsprojects.com/)
- [Chart.js Documentation](https://www.chartjs.org/docs/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Modbus Protocol Specification](https://www.modbus.org/)
- [SDM630 User Manual](https://www.eastroneurope.com/products/view/sdm630modbus)

## 🔮 Future Enhancements

- [ ] WebSocket support for real-time updates
- [ ] Email/SMS alerts for abnormal conditions
- [ ] Historical data analysis and trends
- [ ] Energy cost calculation
- [ ] Multi-device support
- [ ] Mobile app (React Native/Flutter)
- [ ] Advanced analytics and ML predictions
- [ ] Export to PDF reports
- [ ] Integration with Home Assistant
- [ ] RESTful API documentation (Swagger/OpenAPI)

---

**⭐ If you find this project helpful, please give it a star!**

Made with ❤️ for the IoT and Energy Monitoring community
