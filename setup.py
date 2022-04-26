from setuptools import setup

package_name = 'py_servo'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='kazuki-cpu',
    maintainer_email='Kazuki-Fujiwara@outlook.com',
    description='Test program of servo motor control for raspberry pi',
    license='Apache Licence 2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
                'talker = py_servo.servo_publisher:main',
                'listener = py_servo.servo_subscriber:main',
        ],
    },
)
