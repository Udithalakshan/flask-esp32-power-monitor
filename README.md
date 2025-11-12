# ⚡ Three-Phase Power Monitoring System

A complete real-time power monitoring solution that bridges industrial power analyzers with modern web interfaces using ESP32, Flask, and Modbus RTU protocol.


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
Edit the ESP32 code (`esp32_power_monitor/esp32_power_monitor.ino`):
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
2. Install required libraries:
   - ArduinoJson (via Library Manager)
   - ModbusMaster (via Library Manager)
3. Select board: **ESP32 Dev Module**
4. Select correct COM port
5. Upload the code
6. Open Serial Monitor (115200 baud) to verify connection

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
5. Use "Download Data List" button to export CSV

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
│   └── templates/
│       ├── base.html         # Base template
│       ├── login.html        # Login page
│       ├── register.html     # Registration page
│       └── dashboard.html    # Main dashboard
├── instance/
│   └── power_analyzer.db     # SQLite database (auto-created)
├── esp32_power_monitor/
│   └── esp32_power_monitor.ino  # ESP32 Arduino code
├── docs/
│   └── images/               # Documentation images
├── config.py                 # Flask configuration
├── run.py                    # Application entry point
├── requirements.txt          # Python dependencies
├── .env                      # Environment variables (create this)
├── .gitignore               # Git ignore file
├── LICENSE                   # MIT License
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
- Click X button or press ESC to close
- White background for better visibility in fullscreen

### Data Export
- Click "Download Data List" button
- CSV includes all parameters with timestamps
- File naming: `PowerAnalyzer_YYYY-MM-DDTHH-MM.csv`
- Compatible with Excel, Google Sheets, and analysis tools

## 🐛 Troubleshooting

### ESP32 Can't Connect to WiFi
- Verify SSID and password are correct (case-sensitive)
- Check WiFi signal strength
- Ensure 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- Restart ESP32 and router
- Check for special characters in WiFi password

### "Connection Refused" Error
- Verify Flask server is running: `python run.py`
- Check PC's IP address hasn't changed: `ipconfig` (Windows) or `ifconfig` (Mac/Linux)
- Update `FLASK_SERVER_URL` in ESP32 code with correct IP
- Disable Windows Firewall temporarily to test
- Ensure PC and ESP32 are on same network
- Check if port 5000 is not blocked

### No Data in Dashboard
- Check ESP32 Serial Monitor for errors (115200 baud)
- Verify API key matches between ESP32 code and `.env` file
- Test API endpoint: `http://YOUR_IP:5000/api/status` in browser
- Check if user account exists and you're logged in
- Verify Modbus wiring (A to A, B to B)
- Look for error messages in Flask terminal

### Modbus Communication Errors
- Check RS485 wiring (A/B terminals may be swapped)
- Verify SDM630 slave address (default is 1)
- Test with Modbus testing software (ModbusPoll, QModMaster)
- Check baud rate setting (default 9600, 8N1)
- Ensure proper grounding
- Verify RS485 module has termination resistors if needed
- Check if power analyzer is properly powered

### Database Errors
- Delete `instance/power_analyzer.db` and restart
- Check file permissions on instance folder
- Run `flask db upgrade` if using migrations
- Ensure SQLite is properly installed

### Charts Not Updating
- Check browser console for JavaScript errors (F12)
- Clear browser cache and reload (Ctrl+F5)
- Verify data is being received: check Network tab in browser DevTools
- Ensure Flask server is running without errors
- Check if Chart.js library is loaded properly

### CSV Download Not Working
- Check browser download settings
- Ensure pop-ups are not blocked
- Verify there is data to download (check dashboard)
- Try different browser if issue persists

## 🔒 Security Considerations

### Production Deployment

1. **Change SECRET_KEY**: Generate a strong random key
```python
import secrets
print(secrets.token_hex(32))
```

2. **Change API Key**: Use a strong, unique key (minimum 32 characters)

3. **Use HTTPS**: Set up SSL/TLS certificates
   - Use Let's Encrypt for free certificates
   - Configure Nginx as reverse proxy

4. **Enable CORS**: Configure allowed origins
```python
from flask_cors import CORS
CORS(app, resources={r"/api/*": {"origins": "https://yourdomain.com"}})
```

5. **Use Production Database**: PostgreSQL or MySQL
   - Better performance
   - Enhanced security features
   - Backup capabilities

6. **Set DEBUG=False**: In production environment
```python
app.config['DEBUG'] = False
```

7. **Use Gunicorn**: Replace Flask development server
```bash
pip install gunicorn
gunicorn -w 4 -b 0.0.0.0:5000 run:app
```

8. **Add Rate Limiting**: Prevent API abuse
```python
from flask_limiter import Limiter
limiter = Limiter(app, key_func=get_remote_address)
```

9. **Regular Backups**: Automate database backups
```bash
# Cron job for daily backups
0 2 * * * pg_dump power_analyzer > /backup/db_$(date +\%Y\%m\%d).sql
```

10. **Update Dependencies**: Keep libraries up to date
```bash
pip list --outdated
pip install --upgrade package_name
```

### Network Security
- Use VPN for remote access
- Implement firewall rules (allow only necessary ports)
- Disable unused ports
- Use strong WiFi passwords (WPA3 if available)
- Enable MAC address filtering
- Separate IoT devices on VLAN if possible

## 📈 Performance Optimization

### Database Optimization
Add indexes for better query performance:
```sql
-- Create indexes
CREATE INDEX idx_timestamp ON power_reading(timestamp DESC);
CREATE INDEX idx_user_id ON power_reading(user_id);
CREATE INDEX idx_timestamp_user ON power_reading(timestamp DESC, user_id);
```

### Data Cleanup
Implement automatic data cleanup for old records:
```python
# Add to scheduled task (cron or celery)
from datetime import datetime, timedelta
from project.models import PowerReading
from project import db

def cleanup_old_data(days=30):
    cutoff = datetime.now() - timedelta(days=days)
    deleted = PowerReading.query.filter(PowerReading.timestamp < cutoff).delete()
    db.session.commit()
    print(f"Deleted {deleted} old records")
```

### Caching
Consider implementing Redis for caching:
```python
from flask_caching import Cache

cache = Cache(app, config={
    'CACHE_TYPE': 'redis',
    'CACHE_REDIS_URL': 'redis://localhost:6379/0'
})

@cache.cached(timeout=60)
def get_latest_reading():
    return PowerReading.query.order_by(PowerReading.timestamp.desc()).first()
```

### Frontend Optimization
- Minimize JavaScript bundle size
- Use CDN for libraries (Bootstrap, Chart.js)
- Implement lazy loading for charts
- Compress images and assets
- Enable browser caching

## 🤝 Contributing

Contributions are welcome! Please follow these steps:

1. **Fork the repository**
2. **Create a feature branch**: 
   ```bash
   git checkout -b feature/YourFeature
   ```
3. **Commit changes**: 
   ```bash
   git commit -m 'Add YourFeature'
   ```
4. **Push to branch**: 
   ```bash
   git push origin feature/YourFeature
   ```
5. **Open a Pull Request**

### Code Style
- Follow PEP 8 for Python code
- Use meaningful variable names
- Add comments for complex logic
- Update documentation for new features
- Write unit tests for new functionality
- Ensure all tests pass before submitting PR

### Reporting Issues
When reporting issues, please include:
- Description of the problem
- Steps to reproduce
- Expected behavior
- Actual behavior
- System information (OS, Python version, etc.)
- Error messages and logs
- Screenshots if applicable

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2024 Uditha Lakshan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 👨‍💻 Author

**Uditha Lakshan**
- GitHub: [@Udithalakshan](https://github.com/Udithalakshan)
- LinkedIn: [Uditha Lakshan](https://linkedin.com/in/uditha-lakshan-520411278)
- Email: lakshankjhu@gmail.com
- Portfolio: [Coming Soon]

## 🙏 Acknowledgments

- **Flask Community** - For excellent documentation and support
- **Chart.js Team** - For the powerful charting library
- **Bootstrap Team** - For responsive framework
- **ESP32 Community** - For hardware support and examples
- **Eastron/B+G E-Tech** - For SDM630 Modbus protocol documentation
- **Open Source Contributors** - For various libraries and tools used in this project

## 📚 References

### Documentation
- [Flask Documentation](https://flask.palletsprojects.com/) - Web framework
- [Chart.js Documentation](https://www.chartjs.org/docs/) - Charting library
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32) - ESP32 development
- [Modbus Protocol Specification](https://www.modbus.org/) - Industrial protocol
- [SDM630 User Manual](https://www.eastroneurope.com/products/view/sdm630modbus) - Power analyzer

### Tutorials & Resources
- [Flask Mega-Tutorial](https://blog.miguelgrinberg.com/post/the-flask-mega-tutorial-part-i-hello-world) - Miguel Grinberg
- [ESP32 with Modbus](https://randomnerdtutorials.com/esp32-modbus-protocol-arduino-ide/) - Random Nerd Tutorials
- [SQLAlchemy Tutorial](https://docs.sqlalchemy.org/en/14/tutorial/) - Official SQLAlchemy docs

## 🔮 Future Enhancements

### Planned Features
- [ ] WebSocket support for real-time updates without polling
- [ ] Email/SMS alerts for abnormal conditions (over-voltage, under-voltage)
- [ ] Historical data analysis with statistical insights
- [ ] Energy cost calculation with customizable tariffs
- [ ] Multi-device support (multiple ESP32 devices)
- [ ] Mobile app (React Native/Flutter)
- [ ] Advanced analytics and ML-based predictions
- [ ] Export to PDF reports with graphs
- [ ] Integration with Home Assistant/Node-RED
- [ ] RESTful API documentation (Swagger/OpenAPI)
- [ ] Docker containerization
- [ ] Kubernetes deployment support
- [ ] GraphQL API support
- [ ] Real-time notifications via push notifications
- [ ] Multi-language support (i18n)

### Contributions Needed
- Testing on different hardware configurations
- Performance benchmarking
- Security audit
- UI/UX improvements
- Additional chart types (bar, pie charts)
- Dark mode support
- Advanced filtering and search capabilities

## 📞 Support

If you encounter any issues or have questions:

1. **Check Documentation**: Review this README and code comments
2. **Search Existing Issues**: [GitHub Issues](https://github.com/Udithalakshan/three-phase-power-monitor/issues)
3. **Create New Issue**: Provide detailed description with logs and screenshots
4. **Discussions**: Join [GitHub Discussions](https://github.com/Udithalakshan/three-phase-power-monitor/discussions)
5. **Email Support**: lakshankjhu@gmail.com (Response within 48 hours)

### Getting Help
- Include system information (OS, Python version, ESP32 board type)
- Attach relevant logs and error messages
- Describe what you've already tried
- Screenshots are helpful

## 🌟 Star History

[![Star History Chart](https://api.star-history.com/svg?repos=Udithalakshan/three-phase-power-monitor&type=Date)](https://star-history.com/#Udithalakshan/three-phase-power-monitor&Date)

---

**⭐ If you find this project helpful, please give it a star!**

**💬 Have questions? Open an issue or discussion!**

**🤝 Want to contribute? PRs are welcome!**

Made with ❤️ for the IoT and Energy Monitoring Community

---

*Last Updated: January 2025*
