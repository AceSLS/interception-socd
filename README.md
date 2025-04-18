# interception-socd

SOCD cleaner for interception-tools. Also called snap tap by Razer

## Installation

Install interception-tools. Then run:

```bash
git clone "https://github.com/AceSLS/interception-socd"
cd interception-socd
make install
```

Afterwards create config at /etc/interception/udevmon.d/configname.yaml
Here's mine, just change it to fit your needs:

```yaml
- CMD: mux -c vimproved
- JOB:
  - mux -i vimproved | /opt/interception/sls-keyboard-conf | /opt/interception/interception-socd | uinput -c "/etc/interception/keyboard.yaml" -c "/etc/interception/mouse.yaml"
- JOB: intercept -g $DEVNODE | mux -o vimproved
  DEVICE:
    LINK: /dev/input/by-id/usb-Keychron_Keychron_V5_Max-event-kbd
- JOB: intercept -g $DEVNODE | mux -o vimproved
  DEVICE:
    LINK: /dev/input/by-id/usb-Keychron_Keychron_4K_Link-event-mouse
- JOB: intercept -g $DEVNODE | mux -o vimproved
  DEVICE:
    LINK: /dev/input/by-id/usb-Keychron_Keychron_M3_4K-event-mouse
```

## Other tools

Shoutout to [YeetMouse](https://github.com/AndyFilter/YeetMouse) which I can
highly recommend. You should use a linear curve though, everthing else will
probably make you worse instead of better.
Also don't overdo your acceleration, I have mine at 0.1 which is already pretty high
