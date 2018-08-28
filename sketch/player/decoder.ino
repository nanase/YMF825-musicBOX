byte selx = SELX_LR_ENABLE;

extern int16_t sd_position;
extern byte sd_buffer[SD_BUFFER_SIZE];
extern volatile bool ymf825_playing;
extern volatile bool ymf825_next_file;

bool progress() {
  if (!ymf825_playing)
    return true;

  if (ymf825_next_file) {
    ymf825_next_file = false;
    return false;
  }

  sd_read_buffer(1);

  if (sd_position == EOF)
    return false;

  selx = sd_buffer[sd_position] & SELX_MASK;

  // Wait
  if (selx == SELX_WAIT) {
    wait_add((sd_buffer[sd_position] & LENX_MASK) + 1);
    return true;
  }

  // WOPx
  switch (sd_buffer[sd_position] & WOPX_MASK) {
    case WOPX_WRITE_DA_DD:
      if (!write_da_dd((sd_buffer[sd_position] & LENX_MASK) + 1))
        return false;
      break;

    case WOPX_WRITE_SA_DD:
      if (!write_sa_dd((sd_buffer[sd_position] & LENX_MASK) + 1))
        return false;
      break;
      
    case WOPX_BURSTWRITE_TONE:
      if (!burstwrite_tone(((int16_t)sd_buffer[sd_position] & LENX_MASK) + 1))
        return false;
      break;
      
    case WOPX_BURSTWRITE_EQ:
      if (!burstwrite_eq())
        return false;
      break;
  }

  return true;
}

bool write_da_dd(byte length) {
  sd_read_buffer(length * 2);

  if (sd_position == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825_select();
    ymf825_write(
      sd_buffer[sd_position + i * 2 + 0],   // address
      sd_buffer[sd_position + i * 2 + 1]    // data
    );
    ymf825_unselect();
  }

  return true;
}

bool write_sa_dd(byte length) {
  sd_read_buffer(length + 1);

  if (sd_position == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825_select();
    ymf825_write(
      sd_buffer[sd_position + 0],       // address
      sd_buffer[sd_position + i + 1]    // data
    );
    ymf825_unselect();
  }

  return true;
}

bool burstwrite_tone(int16_t length) {
  sd_read_buffer(length * 30 + 6);

  if (sd_position == EOF)
    return false;

  ymf825_select();
  ymf825_burstwrite(
    sd_buffer[sd_position + 0],
    &sd_buffer[sd_position + 1],
    length * 30 + 5
  );
  ymf825_unselect();

  return true;
}

bool burstwrite_eq() {
  sd_read_buffer(16);

  if (sd_position == EOF)
    return false;

  ymf825_select();
  ymf825_burstwrite(
    sd_buffer[sd_position + 0],
    &sd_buffer[sd_position + 1],
    15
  );
  ymf825_unselect();

  return true;
}
