import json
import os
import base64
from PIL import Image
from io import BytesIO


def is_base64(sb):
    try:
        if isinstance(sb, str):
            # If there's any unicode here, an exception will be thrown and the function will return false
            sb_bytes = bytes(sb, 'ascii')
        elif isinstance(sb, bytes):
            sb_bytes = sb
        else:
            raise ValueError("Argument must be string or bytes")
        return base64.b64decode(sb_bytes, validate=True)
    except Exception:
        return False

def save_images_from_json(json_file, output_folder):
    with open(json_file, 'r', encoding='utf-8') as file:  # Ensure UTF-8 encoding
        data = json.load(file)
        result = data.get('result', [])
        for i, page in enumerate(result, start=1):
            base64_string = page.get('type')
            if base64_string == 'image':
                base64_string = page.get('value')
                if base64_string and is_base64(base64_string):
                    image_data = base64.b64decode(base64_string)
                    image = Image.open(BytesIO(image_data))
                    output_path = os.path.join(output_folder, f'img{i}.jpeg')
                    image.save(output_path)
                else:
                    print(f"Invalid Base64 or no 'page_content' for page {i}: {base64_string}")


json_file = "../../../../integrate/zip_evaluate.result"
output_folder = "out_images"

# Create output folder if it does not exist
os.makedirs(output_folder, exist_ok=True)

# Save images
save_images_from_json(json_file, output_folder)
