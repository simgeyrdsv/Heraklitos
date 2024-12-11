from setuptools import find_packages, setup

package_name = 'agv_py'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='melih',
    maintainer_email='melihyasak10@gmail.com',
    description='TODO: Package description',    
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
         'qrcode = agv_py.qrcode:main',
         'interface = agv_py.interface:main',
         'deneyap_motor_controller = agv_py.deneyap_motor_controller:main',
         'deneyap_sensor_controller = agv_py.deneyap_sensor_controller:main',
         'lidar_web_example = agv_py.lidar_web_example:main',
         'web_interface = agv_py.web_interface:main'
        ],
    },
)
