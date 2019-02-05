+++
title = "State-of-the-Art Survey on In-Vehicle Network Communication “CAN-Bus” Security and Vulnerabilities"
date = 2013-07-01T00:00:00
draft = false

# Authors. Comma separated list, e.g. `["Bob Smith", "David Jones"]`.
authors = ["Omid Avatefipour ", "Hafiz Malik", "SSSS", "dddd"]

# Publication type.
# Legend:
# 0 = Uncategorized
# 1 = Conference paper
# 2 = Journal article
# 3 = Manuscript
# 4 = Report
# 5 = Book
# 6 = Book section
publication_types = ["1"]

# Publication name and optional abbreviated version.
publication = "In *IEEE Workshop on Information Forensics and Security, WIFS 2017-Rennes, France*, IEEE."
publication_short = "In *IEEE Workshop on Information Forensics and Security, WIFS 2017-Rennes, France*"

# Abstract and optional shortened version.
abstract = "The Controller Area Network (CAN) bus serves as a legacy protocol for in-vehicle data communication. Simplicity, robustness, and suitability for real-time systems are the salient features of the CAN bus protocol. However, it lacks the basic security features such as massage authentication, which makes it vulnerable to the spoofing attacks. In a CAN network, linking CAN packet to the sender node is a challenging task. This paper aims to address this issue by developing a framework to link each CAN packet to its source. Physical signal attributes of the received packet consisting of channel and node (or device) which contains specific unique artifacts are considered to achieve this goal. Material and design imperfections in the physical channel and digital device, which are the main contributing factors behind the device-channel specific unique artifacts, are leveraged to link the received electrical signal to the transmitter. Generally, the inimitable patterns of signals from each ECUs exist over the course of time that can manifest the stability of the proposed method. Uniqueness of the channel-device specific attributes are also investigated for time-and frequency-domain. Feature vector is made up of both time and frequency domain physical attributes and then employed to train a neural network-based classifier. Performance of the proposed fingerprinting method is evaluated by using a dataset collected from 16 different channels and four identical ECUs transmitting same message. Experimental results indicate that the proposed method achieves correct detection rates of 95.2% and 98.3% for channel and ECU classification, respectively."

# abstract_short = ""

# Is this a featured publication? (true/false)
featured = false

# Projects (optional).
#   Associate this publication with one or more of your projects.
#   Simply enter your project's folder or file name without extension.
#   E.g. `projects = ["deep-learning"]` references 
#   `content/project/deep-learning/index.md`.
#   Otherwise, set `projects = []`.
# projects = ["internal-project"]

# Tags (optional).
#   Set `tags = []` for no tags, or use the form `tags = ["A Tag", "Another Tag"]` for one or more tags.
tags = []

# Links (optional).
url_pdf = "https://ieeexplore.ieee.org/abstract/document/8267643"
url_preprint = "https://ieeexplore.ieee.org/abstract/document/8267643"
# url_code = "#"
# url_dataset = "#"
# url_project = ""
# url_slides = "#"
# url_video = "#"
# url_poster = "#"
# url_source = "#"

# Custom links (optional).
#   Uncomment line below to enable. For multiple links, use the form `[{...}, {...}, {...}]`.
# url_custom = [{name = "Custom Link", url = "http://example.org"}]

# Digital Object Identifier (DOI)
doi = "https://doi.org/10.1109/WIFS.2017.8267643"

# Does this page contain LaTeX math? (true/false)
math = true

# Featured image
# To use, add an image named `featured.jpg/png` to your page's folder. 
[image]
  # Caption (optional)
  # caption = "Image credit: [**Unsplash**](https://unsplash.com/photos/pLCdAaMFLTE)"

  # Focal point (optional)
  # Options: Smart, Center, TopLeft, Top, TopRight, Left, Right, BottomLeft, Bottom, BottomRight
  focal_point = ""
+++

More detail can easily be written here using *Markdown* and $\rm \LaTeX$ math code.
